cmake_minimum_required(VERSION 3.8)
project(complication)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp GETSYM.cpp GETSYM.h BLOCK.cpp BLOCK.h 算符优先.cpp COMMIT.cpp COMMIT.h)
add_executable(complication ${SOURCE_FILES})