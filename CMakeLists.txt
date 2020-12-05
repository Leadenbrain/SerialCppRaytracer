cmake_minimum_required(VERSION 3.10)
project(SerialCppRT)

include_directories(include/)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(SerialCppRT main.cpp)
file(COPY config/ DESTINATION config/)