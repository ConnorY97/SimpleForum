#pragma once
#include "crow.h"
#include "../services/ForumService.h"

inline void setupForumViewRoutes(crow::SimpleApp& app, ForumService& forumService)
{
    CROW_ROUTE(app, "/forum/<int>").methods("GET"_method)
    ([&forumService](const crow::request& req, int forumId) -> crow::response
    {
        Forum forum;
        std::string error;
        if (!forumService.getForumById(forumId, forum, error)) {
            return crow::response{404, "Forum not found: " + error};
        }

        crow::mustache::context ctx({
            {"id", std::to_string(forum.id)},
            {"title", forum.title},
            {"description", forum.description},
            {"createdBy", forum.createdBy},
            {"createdAt", forum.createdAt}
        });

        auto page = crow::mustache::load("viewForum.html").render(ctx);
        return crow::response{page};
    });
}
