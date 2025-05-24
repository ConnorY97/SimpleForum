cmake_minimum_required(VERSION 3.15)

project(SimpleForum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_compile_options(/wd4267 /wd4244 /wd4566)
endif()

if (MSVC)
    file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR}/Debug)
else()
    file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR})
endif()

set(FORUM_CORE_SRC
    src/services/UserService.cpp
    src/utils/UrlUtils.cpp
    external/sqlite3/sqlite3.c
)

add_library(simpleforum_core STATIC ${FORUM_CORE_SRC})
target_include_directories(simpleforum_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/sqlite3
)

set(SIMPLEFORUM_MAIN
    src/main.cpp
)

add_executable(SimpleForum ${SIMPLEFORUM_MAIN})

include(FetchContent)

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/external/.cpm-cache" CACHE STRING "CPM package cache")

FetchContent_Declare(
  CPM
  GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
  GIT_TAG v0.42.0
)
FetchContent_MakeAvailable(CPM)

CPMAddPackage(
    NAME asio
    GITHUB_REPOSITORY chriskohlhoff/asio
    GIT_TAG asio-1-34-2
)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)

CPMAddPackage(
  NAME crow
  GITHUB_REPOSITORY CrowCpp/crow
  GIT_TAG v1.2.1.2
)
add_library(crow INTERFACE)
target_include_directories(crow INTERFACE ${crow_SOURCE_DIR}/include)
target_link_libraries(crow INTERFACE asio)

target_link_libraries(SimpleForum PRIVATE simpleforum_core crow)

if (WIN32)
    target_link_libraries(SimpleForum PRIVATE simpleforum_core ws2_32)
endif()

option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
