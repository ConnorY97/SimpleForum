#pragma once
#include "crow.h"
#include "../services/ForumService.h"

inline void setupForumListRoutes(crow::SimpleApp& app, ForumService& forumService)
{
    CROW_ROUTE(app, "/forums").methods("GET"_method)([&forumService](const crow::request& req)
    {
        std::string error;
        std::vector<Forum> forumData;
        if (!forumService.listForums(forumData, error))
        {
            return crow::response(500, "Failed to load forums: " + error);
        }

        crow::mustache::context ctx;
        std::vector<crow::mustache::context> forumList;

        for (const auto& forum : forumData) {
            crow::mustache::context forumCtx;
            forumCtx["id"] = std::to_string(forum.id);
            forumCtx["title"] = forum.title;
            forumCtx["description"] = forum.description;
            forumCtx["createdBy"] = forum.createdBy;
            forumCtx["createdAt"] = forum.createdAt;
            forumList.push_back(std::move(forumCtx));
        }

        ctx["forums"] = std::move(forumList);
        auto page = crow::mustache::load("listForums.html").render(ctx);
        return crow::response{ page };
    });
}
