#include "UserService.h"

bool UserService::registerUser(const std::string& username, const std::string& password, std::string& error)
{
    if (username.empty() || password.empty())
    {
        error = "Missing username or password";
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    if (users_.count(username)) {
        error = "Username already taken";
        return false;
    }

    users_[username] = password;
    return true;
}

bool UserService::loginUser (const std::string&username, const std::string& password, std::string& error)
{
    if (username.empty() || password.empty())
    {
        error = "Missing username or password";
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    auto user = users_.find(username);
    if (user == users_.end())
    {
        error = "Username does not exist";
        return false;
    }

    if (user->second != password)
    {
        error = "Password is incorrect";
        return false;
    }
    return true;
}

