cmake_minimum_required(VERSION 3.2)

project(CATCHEA)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(EXECUTABLE_NAME catchea)

add_executable(${EXECUTABLE_NAME} "")

include(deps/CMakeLists.txt)

include(src/CMakeLists.txt)

set(RES_DIR ${CMAKE_SOURCE_DIR}/res)
file(COPY ${RES_DIR} DESTINATION ${CMAKE_BINARY_DIR})

include(CreateDist.cmake)
