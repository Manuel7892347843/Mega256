cmake_minimum_required(VERSION 3.20)
project(Mega256 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIB_SOURCES
        src/Mega256.cpp
        include/Mega256.h
)
add_executable(Mega256 main.cpp ${LIB_SOURCES})
