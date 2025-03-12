cmake_minimum_required(VERSION 3.11)
project(cache_all LANGUAGES CXX)

# GoogleTest requires at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(add_gtest TESTNAME)
    # create an executable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})

    set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

    target_compile_definitions(${TESTNAME} PUBLIC
                               -DTEST_DATA_DIR="${TEST_DATA_DIR}")
    # link gtest
    target_link_libraries(${TESTNAME} gtest gtest_main)

    target_include_directories(${TESTNAME} PUBLIC
                           ${PROJECT_SOURCE_DIR}/include
                           ${PROJECT_SOURCE_DIR}/src
                           ${PROJECT_SOURCE_DIR}/test_implementation)

    add_test(${TESTNAME} ${TESTNAME})
endmacro()

add_gtest(cache_test cache_test.cc)

add_gtest(cperfect_cache_test perfect_cache_test.cc)
