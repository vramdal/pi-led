#ifndef LedMatrix_cpp
#define LedMatrix_cpp

#include "LedMatrix.hpp"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../wiringpi/wiringPi/wiringPiSPI.c"
#include "../../wiringpi/wiringPi/wiringPi.c"

static const uint8_t CHIP_0_GPIO_NR = 17;
static const uint8_t CHIP_1_GPIO_NR = 18;
static const uint8_t CHIP_2_GPIO_NR = 27;
static const uint8_t CHIP_3_GPIO_NR = 22;

void LedModule::setChip(uint8_t c, bool useSys){
    chip = c;
    channel = (chip <= 3 ? 0 : 1);
    this->useSys = useSys;
}

void * LedModule::reverseEndian(void *p, size_t size) {
    char *head = (char *)p;
    char *tail = head + size -1;

    for(; tail > head; --tail, ++head) {
        char temp = *head;
        *head = *tail;
        *tail = temp;
    }
    return p;
}
void LedModule::selectChip()
{
/*    digitalWrite(this->useSys ? CHIP_0_GPIO_NR : 0,0);
    digitalWrite(this->useSys ? CHIP_1_GPIO_NR : 1,0);
*/
    switch (chip) {
        case 0:
            digitalWrite(this->useSys ? CHIP_0_GPIO_NR : 0,0);
            digitalWrite(this->useSys ? CHIP_1_GPIO_NR : 1,0);

            break;

        case 1:

            digitalWrite(this->useSys ? CHIP_0_GPIO_NR : 0,0);
            digitalWrite(this->useSys ? CHIP_1_GPIO_NR : 1,1);

            break;

        case 2:
            digitalWrite(this->useSys ? CHIP_0_GPIO_NR : 0,1);
            digitalWrite(this->useSys ? CHIP_1_GPIO_NR : 1,0);
            break;

        case 3:
            digitalWrite(this->useSys ? CHIP_0_GPIO_NR : 0,1);
            digitalWrite(this->useSys ? CHIP_1_GPIO_NR : 1,1);
            break;
        case 4:
            digitalWrite(this->useSys ? CHIP_2_GPIO_NR : 2,0);
            digitalWrite(this->useSys ? CHIP_3_GPIO_NR : 3,0);
            break;
        case 5:
            digitalWrite(this->useSys ? CHIP_2_GPIO_NR : 2,0);
            digitalWrite(this->useSys ? CHIP_3_GPIO_NR : 3,1);
            break;
        case 6:
            digitalWrite(this->useSys ? CHIP_2_GPIO_NR : 2,1);
            digitalWrite(this->useSys ? CHIP_3_GPIO_NR : 3,0);
            break;
        case 7:
            digitalWrite(this->useSys ? CHIP_2_GPIO_NR : 2,1);
            digitalWrite(this->useSys ? CHIP_3_GPIO_NR : 3,1);
            break;

    }

}

void LedModule::clearMatrix()
{
    memset(matrix,0,(width * height) /8);
}

void LedModule::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (y >= height) return;
    if (x >= width) return;


    if (color) {
        *(matrix + x) = *(matrix + x) | (1 << y); // find the bit in the byte that needs to be turned on;
    }else {
        *(matrix + x) = *(matrix + x) & ~(1 << y); // find the bit in the byte that needs to be turned on;
    }
}

void LedModule::drawByte(uint8_t x, uint8_t value) {
    if (x >= width) return;

    *(matrix + x) = value;
}

uint8_t LedModule::scrollMatrixOnce(uint8_t shift) {
    int n;
    uint8_t temp;



    for(n=width-1;n>-1;n--) {
        temp = matrix[n];
        matrix[n] = shift;
        shift = temp;
    }
    return shift;
}

void LedModule::printMatrix(unsigned char output[][129]) {
    uint8_t byte;
    for (int col=0; col < width; col++) {
        byte = matrix[col];
        for (int row=0; row < height; row++) {
            if ((byte &0x80) != 0) {
                output[row][col + (width * chip)] = 248;
            } else {
                output[row][col + (width * chip)] = ' ';
            }

            byte = byte << 1;
        }
    }
}

void LedModule::writeMatrix(){
    int size = width * height / 8;
    uint8_t *output = (uint8_t *) malloc(size+2);
    uint16_t data;
    uint8_t write;

    *output = 160;
    *(output+1) = 0;



    bitarray_copy(matrix, 0, width * height, (output+1), 2);
    cout << "Selecting chip for " << (int) this->chip << endl;
    selectChip();
    //delay(2000);
    //sendCommand(LED_OFF);
    cout << "Writing data for " << (int) this->chip << endl;
    wiringPiSPIDataRW(channel,output,size+1);
    //delay(2000);

    data = WR;
    data <<= 7;
    data |= 63; //last address on screen
    data <<= 4;
    write = (0x0f & *(matrix+31));
    data |= write;
    data <<= 2;

    reverseEndian(&data, sizeof(data));
    //cout << "Writing chip " << (int) this->chip << " value " << (int) matrix[0] << endl;
    wiringPiSPIDataRW(channel, (uint8_t *) &data, 2);
    usleep(500);
    //sendCommand(LED_ON);
    free(output);
}
void LedModule::sendCommand( uint8_t cmd) {
    uint16_t data=0;

    data = COMMAND;

    data <<= 8;
    data |= cmd;
    data <<= 5;


    reverseEndian(&data, sizeof(data));
    selectChip();
    wiringPiSPIDataRW(channel, (uint8_t *) &data, 2);

}

void LedModule::blink(int blinky) {
    if (blinky)
        sendCommand(BLINK_ON);
    else
        sendCommand(BLINK_OFF);
}

void LedModule::setBrightness(uint8_t pwm) {
    if (pwm > 15)
    {
        pwm = 15;
    }

    sendCommand(PWM_CONTROL | pwm);
}
void LedModule::init(){
    cout << "Initializing chip " << int(chip) << " (channel " << int(channel) << ")" << endl;
    sendCommand(SYS_EN);
    sendCommand(LED_ON);
    sendCommand(MASTER_MODE);
    sendCommand(INT_RC);
    sendCommand(COMMON_8NMOS);
    blink(0);
    setBrightness(1);
    cout << "Done initializing chip " << int(chip) << " (channel " << int(channel) << ")" << endl;
}
LedModule::LedModule() {
    chip = -1;
    width = 32;
    height = 8;
}
LedModule::~LedModule() {

}
LedModule::LedModule(uint8_t c, uint8_t w, uint8_t h) {
    chip = c;
    width = w;
    height = h;
}


LedMatrix::LedMatrix(int m, bool useSys) {
    int i;
    moduleNum = m;
    _width = 32*moduleNum;
    //cout <<  "Modules: " <<  int(moduleNum) << ", width " << int(_width) << endl;
    _height = 8;
    modules = new LedModule[m];
    fontWidth = 8;

    cout << "Starting wiringPiSPISetup" << endl;
    //setenv("WIRINGPI_GPIOMEM", "1", 1);
    int spiBusSpeed = 256000;
    //int spiBusSpeed = 10;
    if (wiringPiSPISetup(0, spiBusSpeed) <0)
        cout <<  "SPI Setup Failed for channel 0: " <<  strerror(errno) << endl;

    if (wiringPiSPISetup(1, spiBusSpeed) <0)
        cout <<  "SPI Setup Failed for channel 1: " <<  strerror(errno) << endl;

    if (useSys) {
        cout << "Starting wiringPiSetupSys" << endl;
        if (wiringPiSetupSys() == -1)
            exit(1);
        cout << "wiringPiSetupSys complete" << endl;

    } else {
        cout << "Starting wiringPiSetup" << endl;
        if (wiringPiSetup() == -1)
            exit(1);
        cout << "wiringPiSetup complete" << endl;
    }

    pinMode(useSys ? CHIP_0_GPIO_NR : 0, OUTPUT);
    pinMode(useSys ? CHIP_1_GPIO_NR : 1, OUTPUT);
    pinMode(useSys ? CHIP_2_GPIO_NR : 2, OUTPUT);
    pinMode(useSys ? CHIP_3_GPIO_NR : 3, OUTPUT);


cout << "Initializing " << int(m) << " modules" << endl;
    for (i=0; i < m; i++) {
        modules[i].setChip(i, useSys);
        modules[i].init();
    }
cout << "Done initializing " << int(m) << " modules" << endl;

}

void LedMatrix::init() {
    int i;
    for(i = 0; i < moduleNum; i++) {
        cout << "Initializing module " << (int) i << endl;
        modules[i].init();
    }
}

LedMatrix::~LedMatrix() {
    delete []modules;
}

void LedMatrix::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (y >= _height) return;
    if (x >= _width) return;

    uint8_t m;
    // figure out which matrix controller it is
    m = x / 32;
    x %= 32;
    modules[m].drawPixel(x, y, color);
}

void LedMatrix::drawByte(uint8_t x, uint8_t value) {
    if (x >= _width) {
        cout <<  "Attempting to drawByte off screen at pos " <<  int(x) << ", width is " << int(_width) << endl;
        return;
    }

    uint8_t m;
    // figure out which matrix controller it is
    m = x / 32;
    x %= 32;
    //cout << "Module " << (int) m << ", x " << (int) x << ": " << (int) value << endl;
    modules[m].drawByte(x, value);
}

void LedMatrix::writeMatrix() {

    for (int i=0; i < moduleNum; i++) {
        modules[i].writeMatrix();
    }
    //printMatrix();
}

void LedMatrix::printMatrix() {
    unsigned char output[8][128+1];

    for (int i=0; i < moduleNum; i++) {
        modules[i].printMatrix(output);
    }


    for (int i=0; i < _height; i++ ){
        output[i][_width] = '\0';
        //cout << strlen(output[i]) << endl;
        //cout << output[i] << endl;
        //printf("%s \n",output[i]);
    }
}

void LedMatrix::clearMatrix() {

    for (int i=0; i < moduleNum; i++) {
        modules[i].clearMatrix();
    }
}

void LedMatrix::scrollMatrixOnce(int offset) {
    int n;

    if (offset >= fontWidth)
        return;

    uint8_t temp = offscreen[offset];

    for(n=moduleNum;n>0;n--) {
        temp = modules[n-1].scrollMatrixOnce(temp); // n-1 since the matrix is 0 based
    }


    writeMatrix();
    usleep(12500);
}

void LedMatrix::drawChar(char c, int offset, uint8_t *buf) {
    int row, col,x,y;
    memset(buf,0,8);
    for (row=0; row<8; row++) {
        uint8_t z = fontData[(int8_t) c][row];
        for(col=0; col<8; col++) {
            x = offset * fontWidth + col;
            y = 8 - row;

            if ((z &0x80) != 0) {
                *(buf + x) = *(buf + x) | (1 << y); // find the bit in the byte that needs to be turned on;
            }else {
                *(buf + x) = *(buf + x) & ~(1 << y); // find the bit in the byte that needs to be turned on;
            }

            z = z <<1;
        }
    }
}

void LedMatrix::writeMessage(char *message) {
    int display_len = 16;//width*displays/font_width;
    int i, pix;
    int msg_len = strlen(message);


    for (i=0; i < display_len; i++) {
        modules[i].init();
    }

    clearMatrix();

    for (i=0; i < msg_len; i++ ) {

        char c = message[i];
        drawChar(c, 0, offscreen);

        for (pix = 0; pix < fontWidth; pix++){
            scrollMatrixOnce(pix);
        }
    }

    for (i=0; i < display_len; i++ ) {

        char c = ' ';
        drawChar(c, 0, offscreen);

        for (pix = 0; pix < fontWidth; pix++){
            scrollMatrixOnce(pix);
        }
    }

}

void LedMatrix::setPin(uint8_t pin, uint8_t value) {
    digitalWrite(pin, value);
    cout << "Read pin " << (int) pin << ": " << digitalRead(pin) << endl;
}

#endif