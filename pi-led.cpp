#include <node/node.h>
#include <node/node_object_wrap.h>
#include <v8.h>

// C standard library
#include <cstdlib>
#include <ctime>
#include <errno.h>

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace std;
using namespace v8;
using namespace node;

#include "LedMatrix.hpp"

namespace {

// We use a struct to store information about the asynchronous "work request".
struct Baton {
    // This handle holds the callback function we'll call after the work request
    // has been completed in a threadpool thread. It's persistent so that V8
    // doesn't garbage collect it away while our request waits to be processed.
    // This means that we'll have to dispose of it later ourselves.
    Persistent<Function> callback;
    Persistent<Object> emitter;
   
    //Persistent<String> display_message;
    char *display_message;

    // Tracking errors that happened in the worker function. You can use any
    // variables you want. E.g. in some cases, it might be useful to report
    // an error number.
    bool error;
    std::string error_message;


    // Custom data you can pass through.
    int32_t result;
};

static LedMatrix *matrix;

class PiLed: public ObjectWrap {
public:
  
  static Handle<Value> New(const Arguments& args);
  static Handle<Value> WriteMessage(const Arguments& args);
  static Handle<Value> WriteBytes(const Arguments& args);
  static Handle<Value> ClearMatrix(const Arguments& args);
  static Handle<Value> Init(const Arguments& args);
  static void AsyncWork(uv_work_t* req);
  static void AsyncAfter(uv_work_t* req);
};


Handle<Value> PiLed::New(const Arguments& args) {
  cout << "Initializing PiLed" << endl;
  HandleScope scope;

  assert(args.IsConstructCall());
  PiLed* self = new PiLed();
  self->Wrap(args.This());

  matrix = new LedMatrix(8);

  cout << "Done initializing PiLed" << endl;
  return scope.Close(args.This());
}

Handle<Value> PiLed::ClearMatrix(const Arguments& args) {
  HandleScope scope;
  matrix->clearMatrix();
  matrix->writeMatrix();
  return Undefined();
}

Handle<Value> PiLed::Init(const Arguments& args) {
  HandleScope scope;
  matrix->init();
  return Undefined();
}

Handle<Value> PiLed::WriteBytes(const Arguments& args) {
  HandleScope scope;
  if (args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Arguments: Bytes to display, X-pos")));
  }
  if (!args[0]->IsArray()) {
    return ThrowException(Exception::TypeError(String::New("First argument must be an Array of bytes")));
  }
  if (!args[1]->IsUint32()) {
    return ThrowException(Exception::TypeError(String::New("Second argument must be an integer < 256")));
  }
  uint8_t x = Local<Number>::Cast(args[1])->Uint32Value() & 0x000000ff;
  Handle<Array> array = Handle<Array>::Cast(args[0]);
  if (x + array->Length() > 256) {
    return ThrowException(Exception::TypeError(String::New("Cannot position bitmap off-screen")));
  }
  for (uint32_t i = 0; i < array->Length(); i++) {
     uint32_t val32 = array->Get(i)->ToUint32()->Value();
     uint8_t b = val32 & 0x000000ff;
     uint8_t pos = (i + x) & 0x000000ff;
     matrix->drawByte(pos , b);
  }


  //v8::Array arr(args[0]);
  //uint8_t *bytes[] (byte *) malloc(args[0]->Length());
  //memcpy(arr, bytes);
  /*matrix->drawPixel(0, 0, 255);
  matrix->drawPixel(0, 32, 127);
  matrix->drawPixel(0, 65, 64);
  matrix->drawPixel(0, 80, 64);*/
  matrix->writeMatrix();
  return Undefined();
}


// emits ping event
Handle<Value> PiLed::WriteMessage(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
        // No argument was passed. Throw an exception to alert the user to
        // incorrect usage. Alternatively, we could just use 0.
        return ThrowException(
            Exception::TypeError(String::New("Arguments: String to print, Callback Function"))
        );
    }

    if (!args[1]->IsFunction()) {
        return ThrowException(Exception::TypeError(
            String::New("Second argument must be a callback function")));
    }
     if (!args[0]->IsString()) {
        return ThrowException(Exception::TypeError(
            String::New("First argument must be a String")));
    }
    // There's no ToFunction(), use a Cast instead.
    Local<Function> callback = Local<Function>::Cast(args[1]);

    // The baton holds our custom status information for this asynchronous call,
    // like the callback function we want to call when returning to the main
    // thread and the status information.
    Baton* baton = new Baton();
    baton->error = false;
    baton->callback = Persistent<Function>::New(callback);
    baton->emitter = Persistent<Object>::New(args.This());

    //From: http://create.tpsitulsa.com/wiki/V8/Persistent_Handles
    //Persistent<Value> value = Persistent<Value>::New(String::New("Hello, World"));
    

    v8::String::AsciiValue string(args[0]);
        baton->display_message = (char *) malloc(string.length() + 1);
        strcpy(baton->display_message, *string);
  
    // This creates the work request struct.
    uv_work_t *req = new uv_work_t();
    req->data = baton;

    // Schedule our work request with libuv. Here you can specify the functions
    // that should be executed in the threadpool and back in the main thread
    // after the threadpool function completed.
    int status = uv_queue_work(uv_default_loop(), req, AsyncWork, (uv_after_work_cb)AsyncAfter);
    assert(status == 0);

    return Undefined();
 

  return Undefined();
}

// This function is executed in another thread at some point after it has been
// scheduled. IT MUST NOT USE ANY V8 FUNCTIONALITY. Otherwise your extension
// will crash randomly and you'll have a lot of fun debugging.
// If you want to use parameters passed into the original call, you have to
// convert them to PODs or some other fancy method.
void PiLed::AsyncWork(uv_work_t* req) {
    Baton* baton = static_cast<Baton*>(req->data);



    // Do work in threadpool here.
    //LedMatrix *matrix = new LedMatrix(4);

  matrix->writeMessage(baton->display_message);

 
  baton->result = 42;

    // If the work we do fails, set baton->error_message to the error string
    // and baton->error to true.
}

// This function is executed in the main V8/JavaScript thread. That means it's
// safe to use V8 functions again. Don't forget the HandleScope!
void PiLed::AsyncAfter(uv_work_t* req) {
    HandleScope scope;
    Baton* baton = static_cast<Baton*>(req->data);

    if (baton->error) {
        Local<Value> err = Exception::Error(String::New(baton->error_message.c_str()));

        // Prepare the parameters for the callback function.
        const unsigned argc = 1;
        Local<Value> argv[argc] = { err };

        // Wrap the callback function call in a TryCatch so that we can call
        // node's FatalException afterwards. This makes it possible to catch
        // the exception from JavaScript land using the
        // process.on('uncaughtException') event.
        TryCatch try_catch;
        baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    } else {
        // In case the operation succeeded, convention is to pass null as the
        // first argument before the result arguments.
        // In case you produced more complex data, this is the place to convert
        // your plain C++ data structures into JavaScript/V8 data structures.


    Local<Value> event[2] = {
    String::New("FinishedWrite"), // event name
    String::New("success")  // argument
    };

    MakeCallback(baton->emitter, "emit", 2, event);
   

       const unsigned argc = 2;
        Local<Value> argv[argc] = {
            Local<Value>::New(Null()),
            Local<Value>::New(Integer::New(baton->result))
        };

        // Wrap the callback function call in a TryCatch so that we can call
        // node's FatalException afterwards. This makes it possible to catch
        // the exception from JavaScript land using the
        // process.on('uncaughtException') event.
        TryCatch try_catch;
        baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
        if (try_catch.HasCaught()) {
            node::FatalException(try_catch);
        }
    }
    

    // The callback is a permanent handle, so we have to dispose of it manually.
    baton->callback.Dispose();
    baton->emitter.Dispose();

    //baton->display_message.Dispose();
    free(baton->display_message);

    // We also created the baton and the work_req struct with new, so we have to
    // manually delete both.
    delete baton;
    delete req;
}





void RegisterModule(Handle<Object> target) {
   HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(PiLed::New);
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(String::New("PiLed"));
  NODE_SET_PROTOTYPE_METHOD(t, "WriteMessage", PiLed::WriteMessage);
  NODE_SET_PROTOTYPE_METHOD(t, "WriteBytes", PiLed::WriteBytes);
  NODE_SET_PROTOTYPE_METHOD(t, "ClearMatrix", PiLed::ClearMatrix);
  NODE_SET_PROTOTYPE_METHOD(t, "Init", PiLed::Init);

  target->Set(String::NewSymbol("PiLed"), t->GetFunction());

}

NODE_MODULE(PiLed, RegisterModule);
}





