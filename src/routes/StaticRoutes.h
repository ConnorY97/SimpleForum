#pragma once
#include "crow.h"
#include <fstream>
#include <sstream>
#include <iostream>

// 🔧 Fallback for std::string::ends_with (for C++17 and earlier)
inline bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// ✅ MIME type helper using endsWith()
inline std::string getMimeType(const std::string& path) {
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".png")) return "image/png";
    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
    if (endsWith(path, ".svg")) return "image/svg+xml";
    return "application/octet-stream";
}

inline void setUpStaticRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/public/<path>")
    ([](const crow::request& req, crow::response& res, std::string path) {
        std::string fullPath = "public/" + path;
        std::ifstream file(fullPath, std::ios::binary);

        if (!file.is_open()) {
            res.code = 404;
            res.write("File not found: " + fullPath);
            res.end();
            return;
        }

        std::ostringstream contents;
        contents << file.rdbuf();
        file.close();

        std::string contentType = getMimeType(path);
        res.set_header("Content-Type", contentType);
        res.write(contents.str());
        res.end();
    });
}
