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

    bool createForum(const std::string& title, const std::string& description, const std::string& createdBy, std::string& error);

    std::vector<Forum> listForums(std::string& error);

    // Later add updateForum, deleteForum if needed

private:
    sqlite3* dataBase;
};
