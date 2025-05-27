cmake_minimum_required(VERSION 3.15)

project(SimpleForum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_compile_options(/wd4267 /wd4244 /wd4566)
endif()

# Copy templates to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR})

# Source, header, and template files
file(GLOB_RECURSE FORUM_CORE_SRC
    ${CMAKE_SOURCE_DIR}/src/services/*.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/*.cpp
    ${CMAKE_SOURCE_DIR}/external/sqlite3/sqlite3.c
)

file(GLOB_RECURSE FORUM_HEADERS
    ${CMAKE_SOURCE_DIR}/src/services/*.h
    ${CMAKE_SOURCE_DIR}/src/utils/*.h
)

file(GLOB TEMPLATE_FILES ${CMAKE_SOURCE_DIR}/src/templates/*)
file(GLOB ROUTE_FILES ${CMAKE_SOURCE_DIR}/src/routes/*)

# Group files in Visual Studio
source_group("Source Files" FILES ${FORUM_CORE_SRC})
source_group("Header Files" FILES ${FORUM_HEADERS})
source_group("Route Files" FILES ${ROUTE_FILES})
source_group("Templates" FILES ${TEMPLATE_FILES})

# Create core static library
add_library(simpleforum_core STATIC ${FORUM_CORE_SRC} ${FORUM_HEADERS} ${ROUTE_FILES})

target_include_directories(simpleforum_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/sqlite3
)

# Add template files to dummy target for visibility
add_custom_target(template_files SOURCES ${TEMPLATE_FILES})

# Main application source
set(SIMPLEFORUM_MAIN
    src/main.cpp
)

add_executable(SimpleForum ${SIMPLEFORUM_MAIN})

# External dependencies via CPM
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

# Link everything together
target_link_libraries(SimpleForum PRIVATE simpleforum_core crow)

if (WIN32)
    target_link_libraries(SimpleForum PRIVATE simpleforum_core ws2_32)
endif()

# Enable optional unit tests
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
