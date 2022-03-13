cmake_minimum_required(VERSION 3.15.0)
project(basalt_core VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 17)
include_directories("./include")

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(src_dir "${CMAKE_SOURCE_DIR}/src")
set(sources
    ${src_dir}/NodeId.cpp
    ${src_dir}/Node.cpp
    ${src_dir}/basalt.cpp
    ${src_dir}/misc.cpp
    ${src_dir}/JOAATHash.cpp
    ${src_dir}/sha256.cpp
    ${src_dir}/SHA256Hash.cpp
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

add_library(basalt_core ${sources})

add_executable(test test.cpp)
target_link_libraries(test basalt_core)