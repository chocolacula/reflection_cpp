cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(generator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

add_subdirectory(tests)

include_directories(../include)
include_directories(inja/include)
include_directories(inja/third_party/include)

include_directories(../RE-flex/include)
link_directories(../RE-flex/lib)


set(SOURCES
        parsing/lexers/compiled/cpp_source.yy.cpp
        main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} libreflex.a)
