cmake_minimum_required(VERSION 3.10)
project(server CXX)


file(GLOB SOURCES "src/*.cpp")

add_executable(serve ${SOURCES})

target_link_libraries(serve "${CMAKE_CURRENT_SOURCE_DIR}/../build/libuoserve.a")
target_include_directories(serve PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_compile_options(serve PRIVATE -Wall -Wextra -g) 

set(CMAKE_BUILD_TYPE Debug)
