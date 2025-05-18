cmake_minimum_required(VERSION 3.10)

project(SimpleForum)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_WIN32_WINNT=0x0601)
add_compile_options(/wd4267 /wd4244 /wd4566)

include_directories(external/asio)
include_directories(external/crow/include)

file(COPY ${CMAKE_SOURCE_DIR}/src/templates DESTINATION ${CMAKE_BINARY_DIR}/Debug)

set(FORUM_CORE_SRC
    src/services/UserService.cpp
    src/utils/UrlUtils.cpp
)

add_library(simpleforum_core STATIC ${FORUM_CORE_SRC})
target_include_directories(simpleforum_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

set(SIMPLEFORUM_MAIN
    src/main.cpp
)

add_executable(SimpleForum ${SIMPLEFORUM_MAIN})
target_link_libraries(SimpleForum PRIVATE simpleforum_core)


option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()