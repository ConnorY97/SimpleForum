#pragma once
#include "crow.h"
#include "../utils/ForumUtils.h"

inline void setupCreateForumRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/create-forum").methods("GET"_method)([] {
        auto page = crow::mustache::load("createForum.html").render();
        return crow::response{ page };
        });
}