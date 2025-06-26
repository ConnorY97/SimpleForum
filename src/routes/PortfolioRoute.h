#pragma once
#include "crow.h"
#include "../utils/LoggerUtilities.h"

inline void setupPortfolioRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/portfolio")
        (
            []
            {
                crow::mustache::context ctx;
                auto page = crow::mustache::load("portfolio.html").render(ctx);
                LOGINFO("Loading profolio page");
                return crow::response{ page };
            });
}