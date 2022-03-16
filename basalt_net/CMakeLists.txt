cmake_minimum_required(VERSION 3.0.0)
project(basalt_net VERSION 0.1.0)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
include_directories(include)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set(SRC
    src/basalt_net.cpp
)

add_library(basalt_net ${SRC})

add_executable(net_test test.cpp)
target_link_libraries(net_test basalt_net)