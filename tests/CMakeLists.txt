add_executable(test_login
    test_login.cpp
    ../../src/services/UserService.cpp
    ../../src/utils/UrlUtils.cpp
)

target_include_directories(test_login PRIVATE
    ../../src
    ../crow/crow/include
)

add_test(NAME LoginTest COMMAND test_login)
