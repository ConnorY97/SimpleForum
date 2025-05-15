#include "crow_all.h"
#include <unordered_map>
#include <mutex>
#include <iomanip>
#include <sstream>

std::unordered_map<std::string, std::string> users;
std::mutex user_mutex;

std::unordered_map<std::string, std::string> parse_url_encoded(const std::string& body);
std::string url_decode(const std::string& str);

int main()
{
    crow::SimpleApp app;
    crow::mustache::set_base("templates");

    // Home Page
    CROW_ROUTE(app, "/")([] {
        auto page = crow::mustache::load("fancypage.html").render();
        return crow::response{page};
    });

    // Register Form Page
    CROW_ROUTE(app, "/register").methods("GET"_method)([] {
        auto page = crow::mustache::load("register.html").render();
        return crow::response{page};
    });

    CROW_ROUTE(app, "/register").methods("POST"_method)([](const crow::request& req) {
        auto fields = parse_url_encoded(req.body);

        std::string username = fields["username"];
        std::string password = fields["password"];

        if (username.empty() || password.empty()) {
            return crow::response{400, "Missing username or password"};
        }

        {
            std::lock_guard<std::mutex> lock(user_mutex);
            if (users.count(username)) {
                return crow::response{400, "Username already taken"};
            }
            users[username] = password;
        }

        return crow::response{200, "Registered successfully. <a href='/login'>Go to login</a>"};
    });

    app.port(18080).multithreaded().run();
}

std::unordered_map<std::string, std::string> parse_url_encoded(const std::string& body) {
    std::unordered_map<std::string, std::string> result;
    std::istringstream stream(body);
    std::string pair;

    while (std::getline(stream, pair, '&')) {
        auto pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = url_decode(pair.substr(0, pos));
            std::string value = url_decode(pair.substr(pos + 1));
            result[key] = value;
        }
    }

    return result;
}


std::string url_decode(const std::string& str) {
    std::string result;
    std::istringstream iss(str);
    char ch;

    while (iss.get(ch)) {
        if (ch == '%') {
            std::string hex;
            if (iss.get(ch)) hex += ch;
            if (iss.get(ch)) hex += ch;
            result += static_cast<char>(std::stoi(hex, nullptr, 16));
        } else if (ch == '+') {
            result += ' ';
        } else {
            result += ch;
        }
    }

    return result;
}


