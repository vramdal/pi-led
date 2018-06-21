// C standard library
#include <cstdlib>
#include <ctime>
#include <errno.h>

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

#include "../src/pi-led/LedMatrix.hpp"

using namespace std;



int main(int argc, const char* argv[]) {
  LedMatrix *ledMatrix = new LedMatrix(8);
  char bootMsg[255];
  if (argc == 1) {
    strcpy(bootMsg, "Booting ...");
  } else {
    strcpy(bootMsg, argv[1]);
  }
  cout << "Outputting " << bootMsg << " " << argc;
  ledMatrix->writeMessage(bootMsg);
}