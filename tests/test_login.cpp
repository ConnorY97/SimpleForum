#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/services/UserService.h"

TEST_CASE("Register a new user successfully", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.registerUser("alice", "securepassword", error);

    REQUIRE(result);
    REQUIRE(error.empty());
}

TEST_CASE("Register duplicate username fails", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    REQUIRE(userService.registerUser("bob", "1234", error));
    error.clear();

    bool result = userService.registerUser("bob", "differentpass", error);

    REQUIRE_FALSE(result);
    REQUIRE(error.find("UNIQUE") != std::string::npos); // SQLite duplicate error
}

TEST_CASE("Login succeeds with correct credentials", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    REQUIRE(userService.registerUser("carol", "mypassword", error));
    error.clear();

    bool result = userService.loginUser("carol", "mypassword", error);

    REQUIRE(result);
    REQUIRE(error.empty());
}

TEST_CASE("Login fails with wrong password", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    REQUIRE(userService.registerUser("dave", "pass123", error));
    error.clear();

    bool result = userService.loginUser("dave", "wrongpass", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Password is incorrect");
}

TEST_CASE("Login fails if username doesn't exist", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.loginUser("nonexistent", "anything", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Username does not exist");
}

TEST_CASE("Login fails with empty username", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.loginUser("", "pass", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Missing username or password");
}

TEST_CASE("Login fails with empty password", "[UserService]") {
    UserService userService;
    std::string error;

    REQUIRE(userService.clearUsers(error));
    bool result = userService.loginUser("user", "", error);

    REQUIRE_FALSE(result);
    REQUIRE(error == "Missing username or password");
}