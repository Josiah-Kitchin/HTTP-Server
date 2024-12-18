cmake_minimum_required(VERSION 3.10)
project(server CXX)




file(GLOB SOURCES "src/*.cpp")
add_executable(serve ${SOURCES})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(serve PRIVATE -Wall -Wextra -g) 

set(CMAKE_BUILD_TYPE Debug)
