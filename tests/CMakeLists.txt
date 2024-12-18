cmake_minimum_required(VERSION 3.10)
project(test_server CXX)


file(GLOB SOURCES "../src/*.cpp")
file(GLOB INCLUDES "../include/*.hpp") 
list(FILTER SOURCES EXCLUDE REGEX "main.cpp$")

file(GLOB TEST_SOURCES "*.cpp") 


set(GTEST_DIR ./googletest)  
add_subdirectory(${GTEST_DIR} gtest)

add_executable(test ${SOURCES} ${INCLUDES} ${TEST_SOURCES})

include_directories(/usr/include)   
include_directories(../include) 


target_link_libraries(test PRIVATE 
    gtest
    gtest_main
)

target_compile_options(test PRIVATE -Wall -Wextra -g) 

set(CMAKE_BUILD_TYPE Debug)



