cmake_minimum_required(VERSION 3.0.0)
project(basalt_net VERSION 0.1.0)


include_directories(include)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(basalt_net basalt_net.cpp)

