cmake_minimum_required (VERSION 2.6)
project (Worlds CXX)

set (BIN_DIR bin)
set (SOURCE_DIR src)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable(worlds ${SOURCE_DIR}/main.cpp)
