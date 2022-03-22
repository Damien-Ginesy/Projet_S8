cmake_minimum_required(VERSION 3.15.0)
project(basalt_core VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(src_dir "${CMAKE_SOURCE_DIR}/src")
set(core_sources
    ${src_dir}/NodeId.cpp
    ${src_dir}/Node.cpp
    ${src_dir}/basalt.cpp
    ${src_dir}/misc.cpp
    ${src_dir}/JOAATHash.cpp
    ${src_dir}/sha256.cpp
    ${src_dir}/SHA256Hash.cpp
    ${src_dir}/SpookyHash.cpp
)
set(net_sources
    ${src_dir}/net/basalt_net.cpp
    ${src_dir}/net/Message.cpp
    ${src_dir}/net/Session.cpp
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

add_library(basalt_core ${core_sources})
add_library(basalt_net ${net_sources})
target_link_libraries(basalt_core basalt_net)

add_executable(test test.cpp)
target_link_libraries(test basalt_core)

add_executable(server server.cpp)
target_link_libraries(server basalt_core)

add_executable(client client.cpp)
target_link_libraries(client basalt_core)