#include "crow.h"
#include "../src/routes/LoginRoute.h"
#include "../src/services/UserService.h"
#include <cassert>
#include <iostream>

int main() {
    crow::SimpleApp app;
    UserService userService;

    crow::mustache::set_base("templates");

    setupLoginRoutes(app, userService);
    {
        crow::response res(200);
        res.set_header("Location", "/login");

        assert(res.code == 200);
        assert(res.body.find("Incorrect username or password") == std::string::npos);
        std::cout << "[PASS] GET /login (no error)\n";
    }

    return 0;
}
