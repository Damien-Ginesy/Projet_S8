cmake_minimum_required(VERSION 3.8.0)
project(basalt VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(src_dir "${CMAKE_SOURCE_DIR}/src")
set(core_sources
${src_dir}/misc.cpp
${src_dir}/NodeId.cpp
${src_dir}/rng.cpp
${src_dir}/Node.cpp
${src_dir}/basalt.cpp
${src_dir}/fasthash.c
${src_dir}/JOAATHash.cpp
${src_dir}/sha256.cpp
${src_dir}/SHA256Hash.cpp
${src_dir}/SpookyHash.cpp
${src_dir}/HTTPLogger.cpp
)

set_source_files_properties(${llhttp_sources} PROPERTIES LANGUAGE CXX)
# set_source_files_properties(${llhttp_sources} PROPERTIES LANGUAGE CXX)

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
target_link_libraries(basalt basalt_net)

# EXEC

add_executable(test test.cpp)
target_link_libraries(test basalt)

add_executable(p2p_test p2p_test.cpp)
target_link_libraries(p2p_test basalt)

add_executable(test2 test2.cpp)
target_link_libraries(test2 basalt)

add_executable(ranking_tests ranking_tests.cpp)
target_link_libraries(ranking_tests basalt)