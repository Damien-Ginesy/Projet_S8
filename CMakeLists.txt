cmake_minimum_required(VERSION 3.2)
project(basalt VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(!win32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")    
endif(!win32)

add_subdirectory(core)
include_directories(core/include)
include_directories(bootstrap_server)

add_executable(honest_node honest_node.cpp)
target_link_libraries(honest_node basalt)