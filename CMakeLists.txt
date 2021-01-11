cmake_minimum_required(VERSION 3.10)

project(dbscan_zpn_realvec_len)

find_package(Boost REQUIRED COMPONENTS iostreams)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

link_directories(${Boost_LIBRARY_DIR})

add_executable(dbscan_zpn_realvec_len ${SOURCES})
target_include_directories(dbscan_zpn_realvec_len PUBLIC "include/" ${Boost_INCLUDE_DIRS})
target_link_libraries(dbscan_zpn_realvec_len PUBLIC ${Boost_LIBRARIES})
