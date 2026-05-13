cmake_minimum_required(VERSION 3.16)

project(memory_checker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(memory_tests
    src/memory.cpp
    tests/memory_tests.cpp
)

target_include_directories(memory_tests PRIVATE
    include
)

enable_testing()

add_test(NAME memory_tests COMMAND memory_tests)
