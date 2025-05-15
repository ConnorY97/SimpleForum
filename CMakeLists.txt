cmake_minimum_required(VERSION 3.10)

project(SimpleForum)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/asio)
include_directories(crow/include)

file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR}/Debug)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(SimpleForum ${SOURCES})