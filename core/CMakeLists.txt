cmake_minimum_required(VERSION 3.8.0)
project(basalt_lib VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")


if(!win32)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")    
endif(!win32)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(src_dir "src")
set(core_sources
    ${src_dir}/misc.cpp
    ${src_dir}/NodeId.cpp
    ${src_dir}/rng.cpp
    ${src_dir}/Node.cpp
    ${src_dir}/basalt.cpp
    ${src_dir}/hash64.c
    ${src_dir}/sha256.c
    ${src_dir}/SHA256Hash.cpp
    ${src_dir}/SpookyHash.cpp
    ${src_dir}/HTTPLogger.cpp
)


set(net_sources
    ${src_dir}/llhttp/api.c
    ${src_dir}/llhttp/http.c
    ${src_dir}/llhttp/llhttp.c
    ${src_dir}/net/basalt_net.cpp
    ${src_dir}/misc.cpp
    ${src_dir}/net/Message.cpp
    ${src_dir}/net/Session.cpp
    ${src_dir}/net/HTTPClient.cpp
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# LIBS

add_library(basalt ${core_sources})
add_library(basalt_net ${net_sources})
add_library(byzantine_basalt ${core_sources})
target_link_libraries(basalt basalt_net)
target_link_libraries(byzantine_basalt basalt_net)
target_compile_definitions(byzantine_basalt PUBLIC IS_BYZANTINE=1)

# EXEC

add_executable(test test.cpp)
target_link_libraries(test byzantine_basalt)

add_executable(p2p_test p2p_test.cpp)
target_link_libraries(p2p_test basalt)

add_executable(test2 test2.cpp)
target_link_libraries(test2 basalt)

add_executable(byzantine_test byzantine_test.cpp)
target_link_libraries(byzantine_test byzantine_basalt)

