cmake_minimum_required(VERSION 3.11)
project(cache_all LANGUAGES CXX)

# GoogleTest requires at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GTest
add_subdirectory("./googletest")
mark_as_advanced(BUILD_GTEST BUILD_SHARED_LIBS)
enable_testing()

include_directories(./include)

add_subdirectory(./src cache_all)
add_subdirectory(./tests)
