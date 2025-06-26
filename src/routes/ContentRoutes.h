#pragma once
#include "crow.h"
#include <fstream>
#include <sstream>

inline void setupContentRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/content/<string>")
        ([](const crow::request&, const std::string& filename) {
        std::string fullPath = "public/content/" + filename;

        std::ifstream file(fullPath);
        if (!file.is_open())
            return crow::response(404, "Content not found.");

        std::ostringstream contents;
        contents << file.rdbuf();

        crow::response res{ contents.str() };
        res.add_header("Content-Type", "text/html");
        return res;
            });
}
