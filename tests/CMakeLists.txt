cmake_minimum_required(VERSION 3.14)
project(SimpleForumsTests LANGUAGES CXX)

include(CTest)

include(FetchContent)
FetchContent_Declare(
  CPM
  GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
  GIT_TAG origin/master
)
FetchContent_MakeAvailable(CPM)

CPMAddPackage(
  NAME Catch2
  GITHUB_REPOSITORY catchorg/Catch2
  VERSION 3.5.0
  OPTIONS "CATCH_INSTALL_EXTRAS YES" "CATCH_INSTALL_DOCS NO"
)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

# === test_login ===
add_executable(test_login
  test_login.cpp
)

target_include_directories(test_login PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test_login PRIVATE
  simpleforum_core
  crow
  Catch2::Catch2WithMain
)

catch_discover_tests(test_login
  TEST_PREFIX "UserTests/"
  PROPERTIES LABELS "UserTests"
)

# === test_forums ===
add_executable(test_forums
  test_forums.cpp
)

target_include_directories(test_forums PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test_forums PRIVATE
  simpleforum_core
  crow
  Catch2::Catch2WithMain
)

catch_discover_tests(test_forums
  TEST_PREFIX "ForumTests/"
  PROPERTIES LABELS "ForumTests"
)
