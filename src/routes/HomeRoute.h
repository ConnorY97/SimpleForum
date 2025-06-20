#pragma once
#include "crow.h"
#include "../utils/CookieUtils.h"

inline void setupHomeRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/").methods("GET"_method)
    ([](const crow::request& req) -> crow::response
    {
        crow::mustache::context ctx;

        if (const char* responseParam = req.url_params.get("response"))
        {
            ctx["response"] = responseParam;
        }

        std::string username = getUsernameFromCookie(req);
        bool isLoggedIn = !username.empty();
        bool admin = username == "Admin";

        ctx["isLoggedIn"] = isLoggedIn;
        ctx["username"] = username;
        ctx["admin"] = admin;

        auto page = crow::mustache::load("home.html").render(ctx);
        return crow::response{ page };
    });
}
