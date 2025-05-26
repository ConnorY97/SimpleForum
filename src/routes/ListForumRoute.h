#pragma once
#include "crow.h"
#include "../services/ForumService.h"

inline void setupForumListRoutes(crow::SimpleApp& app, ForumService& forumService)
{
    CROW_ROUTE(app, "/forums").methods("GET"_method)([&forumService](const crow::request& req)
    {
        std::string error;
        auto forums = forumService.listForums(error);
        crow::mustache::context ctx;

        // Method 2: Reserve space and construct directly
        std::vector<crow::mustache::context> forumList;
        forumList.reserve(forums.size());

        for (const auto& forum : forums) {
            forumList.emplace_back();
            auto& forumCtx = forumList.back();
            forumCtx["id"] = forum.id;
            forumCtx["title"] = forum.title;
            forumCtx["description"] = forum.description;
            forumCtx["createdBy"] = forum.createdBy;
            forumCtx["createdAt"] = forum.createdAt;
        }

        ctx["forums"] = std::move(forumList);
        auto page = crow::mustache::load("listForums.html").render(ctx);
        return crow::response{ page };
    });
}