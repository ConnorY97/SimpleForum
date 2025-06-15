#pragma once
#include "sqlite3.h"
#include <string>
#include <vector>
#include "iostream"
#include "../utils/CommentUtils.h"
#include "../managers/DatabaseManager.h"
#include "../utils/LoggerUtilities.h"

class CommentService
{
public:
    CommentService();
    ~CommentService();

    bool addComment(int forumId, const std::string& username, const std::string& commentText, int& commentId, std::string& error);

    bool getCommentsForForum(int forumId, std::vector<Comment>& comments, std::string& error);

    bool getCommentById(int commentId, Comment& comment, std::string& error);

    bool updateCommentById(int commentId, std::string& username, const std::string& newText, std::string& error);

    bool deleteCommentById(int commentId, std::string& error);

    bool clearComments(std::string& error);
private:
    sqlite3* dataBase;
};