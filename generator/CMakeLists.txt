cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(rr_gen VERSION 0.1.0)

add_definitions(-DVERSION="${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

add_subdirectory(tests)

include_directories(../include)
include_directories(inja/include)
include_directories(inja/third_party/include)
include_directories(tclap/include)

include_directories(../RE-flex/include)
link_directories(../RE-flex/lib)

set(LLVM_LINK_COMPONENTS Option Support)

find_package(LLVM REQUIRED)
include_directories(${LLVM_INCLUDE_DIR})
link_directories(${LLVM_LIBRARY_DIR})

set(SOURCES
        main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE LLVM clang-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE reflex)

configure_file(config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yaml COPYONLY)
configure_file(templates/object.inja ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/templates/object.inja COPYONLY)
configure_file(templates/enum.inja   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/templates/enum.inja COPYONLY)
