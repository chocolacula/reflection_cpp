cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(example VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(../include)

include_directories(../RE-flex/include)
link_directories(../RE-flex/lib)

set(SOURCES
    main.cpp)

# add_custom_command(
#     OUTPUT  ${CMAKE_SOURCE_DIR}/proto/cenc_header.pb.cc
#     COMMAND protoc -I${CMAKE_SOURCE_DIR}/proto cenc_header.proto --cpp_out=${CMAKE_SOURCE_DIR}/proto
#     DEPENDS proto/cenc_header.proto
#     COMMENT "Generating sources from .proto files"
# )

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} libreflex.a)
