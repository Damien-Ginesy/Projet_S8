cmake_minimum_required(VERSION 3.2)
project(basalt VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(UNIX)
set(THREADS_PREFER_PTHREAD_FLAG ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")    
message("Compiling for linux")    
endif(UNIX)



add_subdirectory(core)
include_directories(core/include)
include_directories(bootstrap_server)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(bootstrap_server
./bootstrap_server/attack.c
./bootstrap_server/bootstrap_server.c
./bootstrap_server/bootstrap.c
./bootstrap_server/ip.c
./bootstrap_server/socket_IO.c
)

add_executable(honest_node honest_node.cpp)
target_link_libraries(honest_node basalt)
add_executable(malicious_node malicious_node.cpp)
target_link_libraries(malicious_node byzantine_basalt)