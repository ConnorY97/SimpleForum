#pragma once
#include "../managers/DatabaseManager.h"
#include "../utils/ForumUtils.h"
#include "../utils/LoggerUtilities.h"
#include "iostream"
#include "sqlite3.h"
#include <string>
#include <vector>

class ForumService
{
  public:
    ForumService();

    bool createForum(const std::string &title, const std::string &description,
                     const std::string &createdBy, int &forumId, std::string &error);

    bool updateForumById(int forumId, std::string username, const std::string &newTitle,
                         const std::string &newDescription, std::string &error);

    bool listForums(std::vector<Forum> &forums, std::string &error);

    bool getForumById(int id, Forum &forum, std::string &error);

    bool deleteForumById(int id, std::string& error);

    bool clearForums(std::string &error);
};
