#pragma once
#include "crow.h"
#include "../services/UserService.h"
#include "../utils/UrlUtils.h"

inline void setupRegisterRoutes(crow::SimpleApp& app, UserService& userService) {
    CROW_ROUTE(app, "/register").methods("GET"_method)([] {
        auto page = crow::mustache::load("register.html").render();
        return crow::response{page};
    });

    CROW_ROUTE(app, "/register").methods("POST"_method)
    ([&userService](const crow::request& req) {
        auto fields = parse_url_encoded(req.body);
        std::string username = fields["username"];
        std::string password = fields["password"];

        std::string error;
        if (!userService.registerUser(username, password, error)) {
            return crow::response{400, error};
        }

        crow::response res(302);
        res.set_header("Location", "/?response=" + url_encode("Registered successfully"));
        return res;
    });
}
