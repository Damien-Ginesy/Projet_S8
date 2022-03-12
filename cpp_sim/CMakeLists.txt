cmake_minimum_required(VERSION 3.15.0)
project(basalt_cpp_sim VERSION 0.0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(basalt_cpp_sim main.cpp Node.cpp)
add_executable(test test.cpp Node.cpp)
