cmake_minimum_required(VERSION 3.15.0)
project(basalt_core VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
include_directories("./include")
link_directories("./lib")
add_executable(basalt_core main.cpp)
