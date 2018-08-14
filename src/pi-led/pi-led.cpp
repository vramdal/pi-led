#include <node/node.h>

// C standard library
#include <cstdlib>
#include <ctime>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>


using namespace std;
using namespace node;

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Boolean;
using v8::Value;
using v8::Undefined;
using v8::Handle;
using v8::Array;


#include "LedMatrix.hpp"

namespace {

    static LedMatrix *matrix;

    void ClearMatrix(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();
        matrix->clearMatrix();
        matrix->writeMatrix();
        args.GetReturnValue().Set(Undefined(isolate));
    }

    void Init(const FunctionCallbackInfo<Value> &args) {
        cout << "Initializing PiLed" << endl;
        Isolate *isolate = args.GetIsolate();
        //PiLed *self = new PiLed();
        //self->Wrap(args.This());
        if (args.Length() < 2) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Arguments: bool useSys, int spiBusSpeed")));
        }
        if (!args[0]->IsBoolean()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be a boolean")));
        }
        if (!args[1]->IsUint32()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Second argument (spiBusSpeed) must be an integer see (http://wiringpi.com/reference/spi-library/)")));
        }

        matrix = new LedMatrix(8, (bool) (Local<Boolean>::Cast(args[0])->BooleanValue()), (uint32_t) (Local<Number>::Cast(args[0])->Uint32Value()));
        matrix->init();
        cout << "Done initializing PiLed" << endl;
        args.GetReturnValue().Set(Undefined(isolate));
    }

    void SetPin(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Arguments: WiringPi pin number, value")));
        }
        if (!args[0]->IsUint32()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be an integer < 41")));
        }
        if (!args[1]->IsUint32()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Second argument must be an integer < 2")));
        }
        uint8_t pin = (uint8_t) (Local<Number>::Cast(args[0])->Uint32Value() & 0x000000ff);
        uint8_t value = (uint8_t) (Local<Number>::Cast(args[1])->Uint32Value() & 0x000000ff);

        matrix->setPin(pin, value);
        args.GetReturnValue().Set(Undefined(isolate));
    }

    void _DrawByte(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Arguments: x, value")));
        }
        if (!args[0]->IsUint32()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be an integer < 256")));
        }
        if (!args[1]->IsUint32()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Second argument must be an integer < 8")));
        }
        uint8_t x = (uint8_t) (Local<Number>::Cast(args[0])->Uint32Value() & 0x000000ff);
        uint8_t value = (uint8_t) (Local<Number>::Cast(args[1])->Uint32Value() & 0x000000ff);

        matrix->drawByte(x, value);
        args.GetReturnValue().Set(Undefined(isolate));
    }

    void _WriteMatrix(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();
        matrix->writeMatrix();
        args.GetReturnValue().Set(Undefined(isolate));
    }

    void WriteBytes(const FunctionCallbackInfo<Value> &args) {
        Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Arguments: Bytes to display, X-pos")));
        }
        if (!args[0]->IsArray()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be an Array of bytes")));
        }
        if (!args[1]->IsUint32()) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Second argument must be an integer < 256")));
        }
        uint8_t x = (uint8_t) (Local<Number>::Cast(args[1])->Uint32Value() & 0x000000ff);
        Handle<Array> array = Handle<Array>::Cast(args[0]);
        if (x + array->Length() > 256) {
            isolate->ThrowException(
                    Exception::TypeError(String::NewFromUtf8(isolate, "Cannot position bitmap off-screen")));
        }
        for (uint32_t i = 0; i < array->Length(); i++) {
            uint32_t val32 = array->Get(i)->ToUint32()->Value();
            uint8_t b = (uint8_t) (val32 & 0x000000ff);
            uint8_t pos = (uint8_t) ((i + x) & 0x000000ff);
            matrix->drawByte(pos, b);
        }


        matrix->writeMatrix();
        args.GetReturnValue().Set(Undefined(isolate));
    }


    void InitModule(Local<Object> exports) {
        NODE_SET_METHOD(exports, "WriteBytes", WriteBytes);
        NODE_SET_METHOD(exports, "ClearMatrix", ClearMatrix);
        NODE_SET_METHOD(exports, "_DrawByte", _DrawByte);
        NODE_SET_METHOD(exports, "_WriteMatrix", _WriteMatrix);
        NODE_SET_METHOD(exports, "Init", Init);
        NODE_SET_METHOD(exports, "SetPin", SetPin);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, InitModule)

}




