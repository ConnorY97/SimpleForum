#pragma once
#include "crow.h"

inline void setupPortfolioRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/portfolio")
    ([] {
        std::ifstream file("public/portfolio.html");
        if (!file) return crow::response(500, "Portfolio page missing.");

        std::ostringstream contents;
        contents << file.rdbuf();
        return crow::response{ 200, contents.str() };
    });

}