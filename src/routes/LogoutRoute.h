#pragma once
#include "crow.h"

inline void setupLogoutRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/logout").methods("POST"_method)
    ([] {
        crow::response res;
        res.code = 200;
        res.add_header("Set-Cookie", "user=; Max-Age=0; Path=/;");
        res.body = "Logged out";
        return res;
    });
};