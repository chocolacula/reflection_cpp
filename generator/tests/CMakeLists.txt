cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(generator_test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(../../include)
include_directories(../../googletest/googletest/include)
include_directories(../inja/third_party/include)

include_directories(../RE-flex/include)
link_directories(../RE-flex/lib)

add_definitions(-DSOURCE_DIR="${PROJECT_SOURCE_DIR}")

set(SOURCES
  ../parsing/lexers/compiled/cpp_source.yy.cpp
  main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} gtest_main)
target_link_libraries(${PROJECT_NAME} libreflex.a)
