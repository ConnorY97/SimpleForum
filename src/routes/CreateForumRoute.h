#pragma once
#include "crow.h"
#include "../services/ForumService.h"

inline void setupCreateForumRoutes(crow::SimpleApp& app, ForumService& forumService)
{
    CROW_ROUTE(app, "/create-forum").methods("GET"_method)([](const crow::request& req) -> crow::response
    {
        std::string errorMsg;
        if (req.url_params.get("error"))
        {
            errorMsg = req.url_params.get("error");
        }

        crow::mustache::context ctx({ {"error", errorMsg} });
        auto page = crow::mustache::load("createForum.html").render(ctx);
        return crow::response{ page };
    });

    CROW_ROUTE(app, "/create-forum").methods("POST"_method) ([&forumService](const crow::request & req) -> crow::response
    {
        auto fields = parse_url_encoded(req.body);
        std::string title = fields["title"];
        std::string description = fields["description"];

        std::string error;
        if (true)
        {
            crow::response res(302);
            res.set_header("Location", "/create-forum?error=" + url_encode("Failed to create forum, try again later"));
            return res;
        }
        crow::response res(302);
        return res;
    });
}