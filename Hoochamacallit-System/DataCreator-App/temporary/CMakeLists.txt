cmake_minimum_required(VERSION 3.30)
project(temporary C)

set(CMAKE_C_STANDARD 99)

add_executable(temporary DC.c ../../Common/src/MessageQueue.c)
