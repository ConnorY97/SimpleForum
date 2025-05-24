add_executable(test_login
    test_login.cpp
)

target_include_directories(test_login PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test_login PRIVATE simpleforum_core crow)

add_test(NAME LoginTest COMMAND test_login)
