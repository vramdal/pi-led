cmake_minimum_required(VERSION 3.7)
project(pi_led)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        bootMsg/bootMsg.cpp
        bit_array.cpp
        bit_array.hpp
        font.c
        font.h
        LedMatrix.cpp
        LedMatrix.hpp
        pi-led.cpp
        wiringPi.c
        wiringPi.h
        wiringPiSPI.c
        wiringPiSPI.h)

add_executable(pi_led ${SOURCE_FILES})