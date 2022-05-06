cmake_minimum_required(VERSION 3.2)
project(basalt VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(core)
include_directories(core/include)

add_executable(honest_node honest_node.cpp)
target_link_libraries(honest_node basalt)