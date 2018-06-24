cmake_minimum_required(VERSION 2.8.4)
project(ledDisplay)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    ../pi-led/bit_array.cpp
    ../pi-led/font.c
    ../pi-led/LedMatrix.cpp
    ../../wiringpi/wiringPi/wiringPi.c
    ../../wiringpi/wiringPi/wiringPiSPI.c
    ledDisplay.cpp)

add_executable(ledDisplay ${SOURCE_FILES})
