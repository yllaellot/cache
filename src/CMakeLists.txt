cmake_minimum_required(VERSION 3.11)
project(cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cache cache.cc)
target_include_directories(cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(perfect_cache perfect_cache.cc)
target_include_directories(perfect_cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)