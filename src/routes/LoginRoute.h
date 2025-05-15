#pragma once
#include "crow.h"
#include "../services/UserService.h"
#include "../utils/UrlUtils.h"

inline void setupRegisterRoutes(crow::SimpleApp& app, UserService& userService) {
    CROW_ROUTE(app, "/login").methods("GET"_method)([] {
        auto page = crow::mustache::load("login.html").render();
        return crow::response{page};
    });

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([&userService](const crow::request& req) {
        auto fields = parse_url_encoded(req.body);
        std::string username = fields["username"];
        std::string password = fields["password"];

        std::string error;
        if (!userService.registerUser(username, password, error)) {
            return crow::response{400, error};
        }

        return crow::response{200, "Registered successfully. <a href='/login'>Go to login</a>"};
    });
}
