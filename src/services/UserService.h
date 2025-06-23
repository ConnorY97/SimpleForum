#pragma once
#include "../managers/DatabaseManager.h"
#include "../utils/LoggerUtilities.h"
#include "../utils/UserUtils.h"
#include "sqlite3.h"
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class UserService
{
  public:
    UserService();

    bool registerUser(const std::string &username, const std::string &password, std::string &error);
    bool registerUser(const std::string &username, const std::string &password,
                      std::string &confirmPassword, std::string &error);
    bool loginUser(const std::string &username, const std::string &password, std::string &error);
    bool ListUsers(std::vector<User> &users, std::string &error);

    bool clearUsers(std::string &error);

  private:
    std::unordered_map<std::string, std::string> users_;
    std::mutex mutex_;
};
