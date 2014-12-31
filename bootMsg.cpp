#include "pi-led.cpp"

int main() {
  LedMatrix *ledMatrix = new LedMatrix(8);
  char *bootMsg = "Booting ...";
  ledMatrix->writeMessage(bootMsg);
}