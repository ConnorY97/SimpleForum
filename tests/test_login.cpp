#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "sharedData/SharedServiceFixture.h"

TEST_CASE_METHOD(SharedServiceFixture, "Register a new user successfully", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.registerUser("alice", "securepassword", error);

    REQUIRE(result);
    REQUIRE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Register duplicate username fails", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    REQUIRE(userService.registerUser("bob", "1234", error));
    error.clear();

    bool result = userService.registerUser("bob", "differentpass", error);

    REQUIRE_FALSE(result);
    REQUIRE(error.find("UNIQUE") != std::string::npos);
}

TEST_CASE_METHOD(SharedServiceFixture, "Login succeeds with correct credentials", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    REQUIRE(userService.registerUser("carol", "mypassword", error));
    error.clear();

    bool result = userService.loginUser("carol", "mypassword", error);

    REQUIRE(result);
    REQUIRE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Login fails with wrong password", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    REQUIRE(userService.registerUser("dave", "pass123", error));
    error.clear();

    bool result = userService.loginUser("dave", "wrongpass", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Password is incorrect");
}

TEST_CASE_METHOD(SharedServiceFixture, "Login fails if username doesn't exist", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.loginUser("nonexistent", "anything", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Username does not exist");
}

TEST_CASE_METHOD(SharedServiceFixture, "Login fails with empty username", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.loginUser("", "pass", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Missing username or password");
}

TEST_CASE_METHOD(SharedServiceFixture, "Login fails with empty password", "[UserService]")
{
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.loginUser("user", "", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Missing username or password");
}

TEST_CASE_METHOD(SharedServiceFixture, "Username exsits with valid users", "[UserService]")
{
    std::string error;

    std::string username = "TestUser";
    std::string password = "TestPassword";

    REQUIRE(userService.registerUser(username, password, error));
    REQUIRE(error.empty());

    REQUIRE(userService.userExists(username, error));
    REQUIRE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Username does not exist with invalid users", "[SharedService]")
{
    std::string error;
    
    std::string username = "TestUser";
    
    REQUIRE_FALSE(userService.userExists(username, error));
    REQUIRE(error == "TestUser does not exist.");
}
