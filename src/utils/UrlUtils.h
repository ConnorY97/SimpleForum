#pragma once
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::string> parse_url_encoded(const std::string& body);
std::string url_decode(const std::string& str);
std::string url_encode(const std::string& str);
