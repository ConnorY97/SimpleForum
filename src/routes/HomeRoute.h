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

        crow::mustache::context ctx({{"response", responseMsg}});
        auto page = crow::mustache::load("home.html").render(ctx);
        return crow::response{page};
    });
}
