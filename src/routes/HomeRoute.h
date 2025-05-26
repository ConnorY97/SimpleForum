#pragma once
#include "crow.h"
#include "../utils/CookieUtils.h"

inline void setupHomeRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/").methods("GET"_method)
    ([](const crow::request& req) -> crow::response
    {
        std::string responseMsg;
        if (req.url_params.get("response"))
        {
            responseMsg = req.url_params.get("response");
        }

        std::string cookieHeader = req.get_header_value("Cookie");
        std::string username = getUsernameFromCookie(req);

        crow::mustache::context ctx({
            {"response", responseMsg},
            {"isLoggedIn", !username.empty()},
            {"username", username}
        });
        auto page = crow::mustache::load("home.html").render(ctx);
        return crow::response{page};
    });
}
