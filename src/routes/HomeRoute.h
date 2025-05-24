#pragma once
#include "crow.h"

inline void setupHomeRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/").methods("GET"_method)
    ([](const crow::request& req) -> crow::response
    {
        std::string responseMsg;
        if (req.url_params.get("response"))
        {
            responseMsg = req.url_params.get("response");
        }

        std::string cookieHeader = req.get_header_value("Cookie");
        std::string username;
        auto pos = cookieHeader.find("user=");
        if (pos != std::string::npos) {
            auto end = cookieHeader.find(";", pos);
            username = cookieHeader.substr(pos + 5, end - (pos + 5));
        }

        crow::mustache::context ctx({
            {"response", responseMsg},
            {"isLoggedIn", !username.empty()},
            {"username", username}
        });
        auto page = crow::mustache::load("home.html").render(ctx);
        return crow::response{page};
    });
}
