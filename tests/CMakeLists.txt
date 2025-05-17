add_executable(test_login
    test_login.cpp
)

target_include_directories(test_login PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/crow/crow/include
)

target_link_libraries(test_login PRIVATE simpleforum_core)

add_test(NAME LoginTest COMMAND test_login)
