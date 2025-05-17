#pragma once
#include "crow.h"

inline void setupHomeRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/").methods("GET"_method)
    ([](const crow::request& req) -> crow::response
    {
        std::string loginMsg;
        if (req.url_params.get("loggedIn"))
        {
            loginMsg = req.url_params.get("loggedIn");
        }

        crow::mustache::context ctx({{"loggedIn", loginMsg}});
        auto page = crow::mustache::load("home.html").render(ctx);
        return crow::response{page};
    });
}
