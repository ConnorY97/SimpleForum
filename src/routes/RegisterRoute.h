#pragma once
#include "crow.h"
#include "../services/UserService.h"
#include "../utils/UrlUtils.h"

inline void setupRegisterRoutes(crow::SimpleApp& app, UserService& userService)
{
    CROW_ROUTE(app, "/register").methods("GET"_method)(
    [](const crow::request& req) -> crow::response
    {
        crow::mustache::context ctx;

        if (const char* responseParam = req.url_params.get("response"))
        {
            ctx["response"] = responseParam;
        }

        auto page = crow::mustache::load("register.html").render(ctx);
        return crow::response{ page };
    });


    CROW_ROUTE(app, "/register").methods("POST"_method)
    ([&userService](const crow::request& req)
    {
        auto fields = parse_url_encoded(req.body);
        std::string username = fields["username"];
        std::string password = fields["password"];
        std::string confirmPassword = fields["confirmPassword"];

        std::string error;
        if (!userService.registerUser(username, password, confirmPassword, error))
        {
            crow::response res(302);
            res.set_header("Location", "/register?response=" + url_encode(error));
            return res;
        }

        crow::response res(302);
        res.set_header("Location", "/?response=" + url_encode("Registered successfully"));
        res.add_header("Set-Cookie", "user=" + username + "; Path=/; Max-Age=604800; SameSite=Lax");
        return res;
    });
}
