cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

include_directories(frozen/include)

add_subdirectory(googletest)
add_subdirectory(tests)
add_subdirectory(generator)
add_subdirectory(example)
