#pragma once
#include "crow.h"
#include "../services/ForumService.h"
#include "../services/CommentService.h"

inline void setupForumViewRoutes(crow::SimpleApp& app, ForumService& forumService, CommentService& commentService)
{
    CROW_ROUTE(app, "/forum/<int>").methods("GET"_method)
    ([&forumService, &commentService](const crow::request& req, int forumId) -> crow::response
    {
        auto urlParams = crow::query_string(req.url_params);
        std::string errorParam = urlParams.get("error") ? urlParams.get("error") : "";

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

        std::vector<crow::json::wvalue> commentList;
        for (const auto& c : comments)
        {
            crow::json::wvalue cjson;
            cjson["username"] = c.username;
            cjson["comment"] = c.comment;
            cjson["createdAt"] = c.createdAt;
            commentList.push_back(std::move(cjson));
        }
        ctx["comments"] = std::move(commentList);

        if (!errorParam.empty())
        {
            ctx["error"] = errorParam;
        }

        auto page = crow::mustache::load("viewForum.html").render(ctx);
        return crow::response{ page };
    });

    CROW_ROUTE(app, "/forum/<int>").methods("POST"_method)
    ([&forumService, &commentService](const crow::request& req, int forumId) -> crow::response
    {
        auto fields = parse_url_encoded(req.body);
        std::string comment = fields["comment"];

        std::string cookieHeader = req.get_header_value("Cookie");
        std::string username = getUsernameFromCookie(req);

        std::string error;
        if (!commentService.addComment(forumId, username, comment, error))
        {
            crow::response res(302);
            res.set_header("Location", "/forum/" + std::to_string(forumId) + "?error = " + url_encode("Failed to create comment"));
            return res;
        }

        crow::response res(302);
        res.set_header("Location", "/forum/" + std::to_string(forumId));
        return res;
    });
}
