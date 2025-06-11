#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>
#include "iostream"
#include "../utils/ForumUtils.h"

class ForumService
{
public:
    ForumService();
    ~ForumService();

    bool createForum(const std::string& title, const std::string& description, const std::string& createdBy, int& forumId, std::string& error);

    bool updateForumById(int forumId, std::string username, const std::string& newTitle, const std::string& newDescription, std::string& error);

    bool listForums(std::vector<Forum>& forums, std::string& error);

    bool getForumById(int id, Forum& forum, std::string& error);

    bool clearForums(std::string& error);

private:
    sqlite3* dataBase;
};
