#pragma once
#include "crow.h"
#include <string>

inline std::string getUsernameFromCookie(const crow::request &req)
{
    std::string cookieHeader = req.get_header_value("Cookie");
    std::string username;
    auto pos = cookieHeader.find("user=");
    if (pos != std::string::npos)
    {
        auto end = cookieHeader.find(";", pos);
        if (end == std::string::npos)
            end = cookieHeader.length();
        username = cookieHeader.substr(pos + 5, end - (pos + 5));
    }
    return username;
}
