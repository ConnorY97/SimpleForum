#pragma once
#include "crow.h"

inline void setupHomeRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")([] {
        auto page = crow::mustache::load("fancypage.html").render();
        return crow::response{page};
    });
}
