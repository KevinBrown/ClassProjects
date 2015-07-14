cmake_minimum_required(VERSION 3.1)
project(Systems_Software)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu11")

set(SOURCE_FILES Compile.c)
add_executable(Systems_Software ${SOURCE_FILES} Compile.c)