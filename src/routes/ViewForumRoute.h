#pragma once
#include "crow.h"
#include "../services/ForumService.h"
#include "../services/CommentService.h"

inline void setupForumViewRoutes(crow::SimpleApp& app, ForumService& forumService, CommentService& commentService)
{
    CROW_ROUTE(app, "/forum/<int>").methods("GET"_method)
    ([&forumService, &commentService](const crow::request& req, int forumId) -> crow::response
    {
        std::string errorParam = req.url_params.get("error") ? req.url_params.get("error") : "";

        std::string error;
        Forum forum;
        if (!forumService.getForumById(forumId, forum, error))
        {
            return crow::response{ 404, "Forum not found: " + error };
        }

        std::vector<Comment> comments;
        if (!commentService.getCommentsForForum(forumId, comments, error))
        {
            return crow::response{ 500, "Failed to get comments: " + error };
        }

        crow::json::wvalue ctx;
        ctx["id"] = forum.id;
        ctx["title"] = forum.title;
        ctx["description"] = forum.description;
        ctx["createdBy"] = forum.createdBy;
        ctx["createdAt"] = forum.createdAt;

        std::string currentUser = getUsernameFromCookie(req);
        std::string editIdParam = req.url_params.get("edit") ? req.url_params.get("edit") : "";

        std::vector<crow::json::wvalue> commentList;
        for (const auto& c : comments)
        {
            crow::json::wvalue cjson;
            cjson["id"] = c.id;
            cjson["forumId"] = forumId;
            cjson["username"] = c.username;
            cjson["comment"] = c.comment;
            cjson["createdAt"] = c.createdAt;

            if (!editIdParam.empty() && std::to_string(c.id) == editIdParam)
            {
                cjson["isEditing"] = true;
            }
            else if (c.username == currentUser)
            {
                cjson["canEdit"] = true;
                cjson["canDelete"] = true;
            }

            commentList.push_back(std::move(cjson));
        }

        ctx["comments"] = std::move(commentList);
        if (!errorParam.empty())
            ctx["error"] = errorParam;

        auto page = crow::mustache::load("viewForum.html").render(ctx);
        return crow::response{ page };
    });

    CROW_ROUTE(app, "/forum/<int>").methods("POST"_method)
    ([&forumService, &commentService](const crow::request& req, int commentId)->crow::response
    {
        auto fields = parse_url_encoded(req.body);
        std::string comment = fields["comment"];
        std::string username = getUsernameFromCookie(req);

        std::string error;
        if (!commentService.addComment(commentId, username, comment, error))
        {
            return crow::response(400, error);
        }

        return crow::response(200);
    });

    CROW_ROUTE(app, "/comment/<int>/edit").methods("POST"_method)
    ([&commentService](const crow::request& req, int commentId)
    {
        auto fields = parse_url_encoded(req.body);
        std::string updated = fields["updatedComment"];
        std::string username = getUsernameFromCookie(req);

        std::string error;
        if (!commentService.updateCommentById(commentId, username, updated, error))
        {
            return crow::response(400, error);
        }

        return crow::response(200);
    });
}
