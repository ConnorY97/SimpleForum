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

    bool addComment(int formId, const std::string& username, const std::string& commentText, std::string& error);

    bool getCommentsForForum(int forumId, std::vector<Comment>& comments, std::string& error);

    bool getCommentById(int commentId, Comment& comment, std::string& error);

    bool updateCommentById(int commentId, const std::string& newText, std::string& error);

    bool clearComments(std::string& error);
private:
    sqlite3* dataBase;
};