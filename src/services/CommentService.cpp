#include "CommentService.h"

CommentService::CommentService()
{
    if (sqlite3_open("forum.db", &dataBase)) // use same DB as forums
    {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(dataBase) << std::endl;
    }

    const char* create_comments_sql =
        "CREATE TABLE IF NOT EXISTS comments ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "forumId INTEGER NOT NULL, "
        "username TEXT NOT NULL, "
        "comment TEXT NOT NULL, "
        "createdAt DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(forumId) REFERENCES forums(id) ON DELETE CASCADE);";

    char* errMsg = nullptr;

    if (sqlite3_exec(dataBase, create_comments_sql, 0, 0, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Failed to create comments table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    std::cout << "Initialized Comments Database" << std::endl;
}

CommentService::~CommentService()
{
    if (dataBase) {
        sqlite3_close(dataBase);
    }
}

bool CommentService::addComment(int forumId, const std::string& username, const std::string& commentText, int& commentId, std::string& error)
{
    const char* sql = "INSERT INTO comments (forumId, username, comment, createdAt) VALUES (?, ?, ?, datetime('now'));";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_int(stmt, 1, forumId);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, commentText.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    commentId = static_cast<int>(sqlite3_last_insert_rowid(dataBase));

    sqlite3_finalize(stmt);
    return true;
}

bool CommentService::getCommentsForForum(int forumId, std::vector<Comment>& comments, std::string& error)
{
    const char* sql = "SELECT id, username, comment, createdAt FROM comments WHERE forumId = ? ORDER BY createdAt ASC;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_int(stmt, 1, forumId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Comment comment;
        comment.id = sqlite3_column_int(stmt, 0);
        comment.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        comment.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        comment.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        comments.push_back(comment);
    }

    sqlite3_finalize(stmt);
    return true;
}

bool CommentService::getCommentById(int commentId, Comment& comment, std::string& error)
{
    const char* sql = "SELECT id, username, comment, createdAt FROM comments WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_int(stmt, 1, commentId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        comment.id = sqlite3_column_int(stmt, 0);
        comment.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        comment.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        comment.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        sqlite3_finalize(stmt);
        return true;
    }
    else {
        error = "Comment not found";
        sqlite3_finalize(stmt);
        return false;
    }
}

bool CommentService::updateCommentById(int commentId, std::string& username, const std::string& newText, std::string& error)
{
    const char* sql = "UPDATE comments SET comment = ? WHERE id = ? AND createdBy = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_text(stmt, 1, newText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, commentId);
    sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    int changes = sqlite3_changes(dataBase);
    sqlite3_finalize(stmt);

    if (changes == 0)
    {
        error = "No comment updated. You may not be the owner or the comment does not exist.";
        return false;
    }

    return true;
}


bool CommentService::deleteCommentById(int commentId, std::string& error)
{
    const char* sql = "DELETE FROM comments WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_int(stmt, 1, commentId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}


bool CommentService::clearComments(std::string& error)
{
    const char* sql = "DELETE FROM comments";
    char* errMsg = nullptr;

    if (sqlite3_exec(dataBase, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        error = std::string("Failed to clear comments: ") + errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
