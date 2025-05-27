#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>
#include "iostream"
#include "../utils/CommentUtils.h"

class CommentService
{
public:
    CommentService();
    ~CommentService();

    bool addComment(int id, const std::string& username, const std::string& commentText, std::string& error);

    bool getCommentsForForum(int id, std::vector<Comment>& comments, std::string& error);

private:
    sqlite3* dataBase;
};