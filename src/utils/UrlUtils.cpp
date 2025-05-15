#include "UrlUtils.h"
#include <sstream>
#include <iomanip>

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
