cmake_minimum_required(VERSION 3.10)
project(uoserve_http CXX)




file(GLOB SOURCES "src/*.cpp")
add_library(uoserve STATIC ${SOURCES})

target_include_directories(uoserve PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_BUILD_TYPE Debug)
