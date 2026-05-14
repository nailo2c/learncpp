cmake_minimum_required(VERSION 3.16)

project(misaligned_pointer_regression LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(runner
    runner.cpp
)

target_include_directories(runner PRIVATE
    /usr/local/include
)

target_link_directories(runner PRIVATE
    /usr/local/lib
)

target_link_libraries(runner PRIVATE
    wasmedge
)

target_link_options(runner PRIVATE
     -Wl,-rpath,/usr/local/lib
)
