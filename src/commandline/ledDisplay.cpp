//
// Created by Vidar Ramdal on 20.06.2018.
//

#ifndef ledDisplay_cpp
#define ledDisplay_cpp

#include "../pi-led/LedMatrix.hpp"
#include "../pi-led/LedMatrix.cpp"
#include "../../wiringpi/wiringPi/wiringPi.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static LedMatrix *matrix;

void Init(bool useSys) {
    cout << "Initializing PiLed" << endl;
    matrix = new LedMatrix(8, useSys);
    matrix->init();
    cout << "Done initializing PiLed" << endl;
}

void ClearMatrix() {
    matrix->clearMatrix();
    matrix->writeMatrix();
}

int WriteBytes(char* buf) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t val32 = (uint32_t) buf[i];
        uint8_t b = (uint8_t) (val32 & 0x000000ff);
        uint8_t pos = (uint8_t) (i & 0x000000ff);
        matrix->drawByte(pos, b);
    }

    matrix->writeMatrix();
    return 0;
}

int main() {
    Init(true);
    char buf[256];
    fgets(buf, sizeof buf, stdin);
    return WriteBytes(buf);
}

#endif