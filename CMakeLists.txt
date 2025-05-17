cmake_minimum_required(VERSION 3.10)

project(SimpleForum)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_WIN32_WINNT=0x0601)
add_compile_options(/wd4267 /wd4244)

include_directories(src/asio)
include_directories(crow/include)

file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR}/Debug)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

enable_testing()
add_subdirectory(tests)

add_executable(SimpleForum ${SOURCES})
