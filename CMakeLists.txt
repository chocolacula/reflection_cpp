cmake_minimum_required(VERSION 3.10)
project(required_reflection VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)
add_subdirectory(tests)
add_subdirectory(generator)
add_subdirectory(example)
