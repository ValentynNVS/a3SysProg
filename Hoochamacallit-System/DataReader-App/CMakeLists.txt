cmake_minimum_required(VERSION 3.30)
project(DataReader_App C)

set(CMAKE_C_STANDARD 99)

add_executable(DataReader_App DR.c ../Common/src/MessageQueue.c)