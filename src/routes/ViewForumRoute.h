#pragma once
#include "crow.h"
#include "../services/ForumService.h"
#include "../services/CommentService.h"

inline void setupForumViewRoutes(crow::SimpleApp& app, ForumService& forumService, CommentService& commentService)
{
    CROW_ROUTE(app, "/forum/<int>").methods("GET"_method)
        ([&forumService, &commentService](const crow::request& req, int forumId) -> crow::response
            {
                Forum forum;
                std::string error;
                if (!forumService.getForumById(forumId, forum, error)) {
                    return crow::response{ 404, "Forum not found: " + error };
                }

                // Get comments
                std::vector<Comment> comments;
                if (!commentService.getCommentsForForum(forumId, comments, error)) {
                    return crow::response{ 500, "Failed to get comments: " + error };
                }

                crow::json::wvalue ctx;
                ctx["id"] = forum.id;
                ctx["title"] = forum.title;
                ctx["description"] = forum.description;
                ctx["createdBy"] = forum.createdBy;
                ctx["createdAt"] = forum.createdAt;

                // Prepare comments
                std::vector<crow::json::wvalue> commentList;
                for (const auto& c : comments) {
                    crow::json::wvalue cjson;
                    cjson["username"] = c.username;
                    cjson["comment"] = c.comment;
                    cjson["createdAt"] = c.createdAt;
                    commentList.push_back(std::move(cjson));
                }
                ctx["comments"] = std::move(commentList);

                auto page = crow::mustache::load("viewForum.html").render(ctx);
                return crow::response{ page };
            });
}

