#pragma once
#include <unordered_map>
#include <mutex>
#include <string>

class UserService
{
public:
    bool registerUser(const std::string& username, const std::string& password, std::string& error);
    bool loginUser (const std::string&username, const std::string& password, std::string& error);

private:
    std::unordered_map<std::string, std::string> users_;
    std::mutex mutex_;
};
