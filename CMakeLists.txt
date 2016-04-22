cmake_minimum_required(VERSION 3.4)
project(nedobash)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c89 -Wall -Wextra")
set(SOURCE_FILES src/plugin_load.c src/exec.c src/command_adder.c src/command_parser.c src/main.c src/help.c)
add_executable(nedobash ${SOURCE_FILES})
target_link_libraries(nedobash ${CMAKE_DL_LIBS})


set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c89 -Wall -Wextra")
set(SOURCE_FILES src/bc.c)
add_library(bc SHARED ${SOURCE_FILES})

