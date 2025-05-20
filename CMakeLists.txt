cmake_minimum_required(VERSION 3.15)

project(SimpleForum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_WIN32_WINNT=0x0601)
add_compile_options(/wd4267 /wd4244 /wd4566)

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
target_link_libraries(SimpleForum PRIVATE simpleforum_core ws2_32)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(simpleforum_core PRIVATE unofficial::sqlite3::sqlite3)

find_package(crow CONFIG REQUIRED)
target_include_directories(SimpleForum PRIVATE ${crow_INCLUDE_DIRS})

set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
target_include_directories(SimpleForum PRIVATE "${VCPKG_ROOT}/installed/x64-windows/include")

option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Set output directories
set_target_properties(SimpleForum PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

# Add a convenient run target
add_custom_target(run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Debug/SimpleForum.exe
    DEPENDS SimpleForum
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug
)
