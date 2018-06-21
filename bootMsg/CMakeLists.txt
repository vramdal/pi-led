cmake_minimum_required(VERSION 2.8.4)
project(bootMsg)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    ../src/pi-led/bit_array.cpp
    ../src/pi-led/bit_array.hpp
    ../src/pi-led/font.h
    ../src/pi-led/font.c
        ../src/pi-led/LedMatrix.hpp
    ../src/pi-led/LedMatrix.cpp
    bootMsg.cpp)

add_executable(bootMsg ${SOURCE_FILES})
