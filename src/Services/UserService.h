#pragma once
#include <unordered_map>
#include "sqlite3.h"
#include <mutex>
#include <string>
#include <iostream>

class UserService
{
public:
    UserService();
    ~UserService();

    bool registerUser(const std::string& username, const std::string& password, std::string& error);
    bool loginUser (const std::string&username, const std::string& password, std::string& error);

private:
    std::unordered_map<std::string, std::string> users_;
    std::mutex mutex_;
    sqlite3* dataBase = nullptr;
};
