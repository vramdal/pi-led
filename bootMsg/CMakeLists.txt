cmake_minimum_required(VERSION 2.8.4)
project(bootMsg)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    ../bit_array.cpp
    ../bit_array.hpp
    ../font.h
    ../font.c
    ../wiringPi.c
    ../wiringPi.h
    ../wiringPiSPI.c
    ../wiringPiSPI.h
    ../LedMatrix.hpp
    ../LedMatrix.cpp
    bootMsg.cpp)

add_executable(bootMsg ${SOURCE_FILES})
