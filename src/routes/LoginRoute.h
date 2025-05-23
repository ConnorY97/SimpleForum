#pragma once
#include "crow.h"
#include "../services/UserService.h"
#include "../utils/UrlUtils.h"

inline void setupLoginRoutes(crow::SimpleApp& app, UserService& userService)
{
    CROW_ROUTE(app, "/login").methods("GET"_method)
    ([](const crow::request& req) -> crow::response
    {
        std::string errorMsg;
        if (req.url_params.get("error"))
        {
            errorMsg = req.url_params.get("error");
        }

        crow::mustache::context ctx({{"success", errorMsg}});
        auto page = crow::mustache::load("login.html").render(ctx);
        return crow::response{page};
    });

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([&userService](const crow::request& req) -> crow::response {
        auto fields = parse_url_encoded(req.body);
        std::string username = fields["username"];
        std::string password = fields["password"];

        std::string error;
        if (!userService.loginUser(username, password, error))
        {
            crow::response res(302);
            res.set_header("Location", "/login?error=" + url_encode("Incorrect username or password"));
            return res;
        }

        crow::response res(302);
        res.set_header("Location", "/?response=" + url_encode("Logged in successfully"));

        // Setting a cookie
        res.add_header("Set-Cookie", "user=" + username + "; Path=/; HttpOnly");

        return res;
    });
}