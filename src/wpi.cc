#include "wpi.h"
#include <node.h>

NODE_MODULE_INIT() {
  #if NODE_VERSION_AT_LEAST(0, 11, 0)
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
  #endif
  INIT(wiringPi);
  INIT(wiringPiSPI);
}

NODE_MODULE_DECLARE(wiringPi);