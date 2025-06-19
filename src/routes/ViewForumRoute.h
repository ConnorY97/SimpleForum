#pragma once
#include "crow.h"
#include "../services/ForumService.h"
#include "../services/CommentService.h"

inline void setupForumViewRoutes(crow::SimpleApp& app, ForumService& forumService, CommentService& commentService)
{
    // View forum
    CROW_ROUTE(app, "/forum/<int>").methods("GET"_method)
    ([&forumService, &commentService](const crow::request& req, int forumId) -> crow::response
    {
        std::string errorParam = req.url_params.get("error") ? req.url_params.get("error") : "";
        std::string error;
        std::string currentUser = getUsernameFromCookie(req);
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

        if (forum.createdBy == currentUser)
        {
            ctx["canEditPost"] = true;
        }

        std::vector<crow::json::wvalue> commentList;
        for (const auto& c : comments)
        {
            crow::json::wvalue cjson;
            cjson["id"] = c.id;
            cjson["forumId"] = forumId;
            cjson["username"] = c.username;
            cjson["comment"] = c.comment;
            cjson["createdAt"] = c.createdAt;

            if (c.username == currentUser)
            {
                cjson["canEditComment"] = true;
            }

            commentList.push_back(std::move(cjson));
        }

        ctx["comments"] = std::move(commentList);
        if (!errorParam.empty())
            ctx["error"] = errorParam;

        auto page = crow::mustache::load("viewForum.html").render(ctx);
        return crow::response{ page };
    });

    // Add comments
    CROW_ROUTE(app, "/forum/<int>").methods("POST"_method)
    ([&forumService, &commentService](const crow::request& req, int forumId)->crow::response
    {
        auto fields = parse_url_encoded(req.body);
        std::string comment = fields["comment"];
        std::string username = getUsernameFromCookie(req);

        std::string error;
        int commentId;
        if (!commentService.addComment(forumId, username, comment, commentId, error))
        {
            LOGERROR(error);
            return crow::response(400, error);
        }

        LOGINFO(username + " created a new comment on forum/" + std::to_string(forumId));
        return crow::response(200);
    });

    // Edit post
    CROW_ROUTE(app, "/forum/<int>/edit").methods("POST"_method)([&forumService](const crow::request& req, int forumId)
    {
        auto fields = parse_url_encoded(req.body);
        std::string updatedPost = fields["updatedPost"];
        std::string updatedDescription = fields["updatedDescription"];
        std::string username = getUsernameFromCookie(req);

        std::string error;
        if (!forumService.updateForumById(forumId, username, updatedPost, updatedDescription, error))
        {
            LOGERROR(error);
            return crow::response(400, error);
        }

        LOGINFO(username + " editted forum/" + std::to_string(forumId));
        return crow::response(200);
    });

    // Edit comments
    CROW_ROUTE(app, "/comment/<int>/edit").methods("POST"_method)
    ([&commentService](const crow::request& req, int commentId)
    {
        auto fields = parse_url_encoded(req.body);
        std::string updated = fields["updatedComment"];
        std::string username = getUsernameFromCookie(req);

        std::string error;
        if (!commentService.updateCommentById(commentId, username, updated, error))
        {
            LOGERROR(error);
            return crow::response(400, error);
        }

        LOGINFO(username + " editted comment");
        return crow::response(200);
    });

    // Delete comments
    CROW_ROUTE(app, "/delete/<int>").methods("POST"_method)
    ([&commentService](const crow::request& req, int commentId)
    {
        std::string error;
        std::string username = getUsernameFromCookie(req);
        if (!commentService.deleteCommentById(commentId, error))
        {
            LOGERROR(error);
            return crow::response(400, error);
        }

        LOGINFO(username + " deleted a comment");
        return crow::response(200);
    });
}
