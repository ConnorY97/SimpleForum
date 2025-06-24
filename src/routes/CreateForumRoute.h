#pragma once
#include "../services/ForumService.h"
#include "../utils/CookieUtils.h"
#include "../utils/UrlUtils.h"
#include "crow.h"

inline void setupCreateForumRoutes(crow::SimpleApp &app, ForumService &forumService)
{
    CROW_ROUTE(app, "/create-forum")
        .methods("GET"_method)(
            [](const crow::request &req) -> crow::response
            {
                std::string errorMsg;
                if (req.url_params.get("error"))
                {
                    errorMsg = req.url_params.get("error");
                }

                crow::mustache::context ctx({{"error", errorMsg}});
                LOGERROR(errorMsg);
                auto page = crow::mustache::load("createForum.html").render(ctx);
                return crow::response{page};
            });

    CROW_ROUTE(app, "/create-forum")
        .methods("POST"_method)(
            [&forumService](const crow::request &req) -> crow::response
            {
                auto fields = parse_url_encoded(req.body);
                std::string title = fields["title"];
                std::string description = fields["description"];
                std::string username = getUsernameFromCookie(req);

                std::string error;
                int forumId;
                if (!forumService.createForum(title, description, username, forumId, error))
                {
                    crow::response res(302);
                    res.set_header("Location",
                                   "/create-forum?error=" +
                                       url_encode("Failed to create forum, try again later"));
                    LOGERROR(error);
                    return res;
                }

                crow::response res(302);
                res.set_header("Location", "/forum/" + std::to_string(forumId));
                LOGINFO(username + " created new forum/" + std::to_string(forumId));
                return res;
            });
}