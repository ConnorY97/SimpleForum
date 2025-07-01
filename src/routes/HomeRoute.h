#pragma once
#include "../services/UserService.h"
#include "../utils/CookieUtils.h"
#include "crow.h"

inline void setupHomeRoutes(crow::SimpleApp &app, UserService& userService)
{
    CROW_ROUTE(app, "/").methods("GET"_method)(
        [&userService](const crow::request &req) -> crow::response
        {
            crow::mustache::context ctx;
            std::string error;
            if (const char *responseParam = req.url_params.get("response"))
            {
                ctx["response"] = responseParam;
            }

            bool isLoggedIn = false;
            std::string username = getUsernameFromCookie(req);
            if (!username.empty())
            {
                if (userService.userExists(username, error))
                {
                    isLoggedIn = true;
                }
                else
                {
                    LOGERROR(error);
                }
            }

            if (const char* success = req.url_params.get("forumDeleteSuccess"))
            {
                if (std::string(success) == "true")
                    ctx["forumDeleteSuccess"] = true;
            }

            bool admin = username == "Admin";

            ctx["isLoggedIn"] = isLoggedIn;
            ctx["username"] = username;
            ctx["admin"] = admin;

            auto page = crow::mustache::load("home.html").render(ctx);
            return crow::response{page};
        });
}
