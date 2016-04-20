cmake_minimum_required(VERSION 3.4)
project(nedobash)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c89 -Wall")
set(SOURCE_FILES main.c)
add_executable(nedobash ${SOURCE_FILES})
