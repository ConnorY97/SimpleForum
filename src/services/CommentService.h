#pragma once
#include "../managers/DatabaseManager.h"
#include "../utils/CommentUtils.h"
#include "../utils/LoggerUtilities.h"
#include "iostream"
#include "sqlite3.h"
#include <string>
#include <vector>

class CommentService
{
  public:
    CommentService();

    bool addComment(int forumId, const std::string &username, const std::string &commentText,
                    int &commentId, std::string &error);

    bool getCommentsForForum(int forumId, std::vector<Comment> &comments, std::string &error);

    bool getCommentById(int commentId, Comment &comment, std::string &error);

    bool updateCommentById(int commentId, std::string &username, const std::string &newText,
                           std::string &error);

    bool deleteCommentById(int commentId, std::string &error);

    bool clearComments(std::string &error);
};