#pragma once
#include "crow.h"
#include "../services/UserService.h"
#include "../utils/CookieUtils.h"

inline void setupAdminRoutes(crow::SimpleApp& app, UserService& userService)
{
    CROW_ROUTE(app, "/admin/users").methods("GET"_method)
    ([&userService](const crow::request& req)
    {
        std::string username = getUsernameFromCookie(req);
        if (username != "Admin") {
            return crow::response(403, "Forbidden: Admin access only.");
        }

        std::vector<User> users;
        std::string error;
        if (!userService.ListUsers(users, error)) {
            LOGERROR(error);
            return crow::response(500, "Error fetching users: " + error);
        }

        crow::json::wvalue ctx;
        std::vector<crow::json::wvalue> userList;
        for (const auto& user : users) {
            crow::json::wvalue u;
            u["username"] = user.username;
            u["password"] = user.password;
            userList.push_back(std::move(u));
        }
        ctx["users"] = std::move(userList);

        auto page = crow::mustache::load("viewUsers.html").render(ctx);
        return crow::response{ page };
    });
}
