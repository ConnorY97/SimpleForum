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

bool CommentService::addComment(int forumId, const std::string& username, const std::string& commentText, std::string& error)
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

    sqlite3_finalize(stmt);
    return true;
}

bool CommentService::getCommentsForForum(int forumId, std::vector<Comment>& comments, std::string& error)
{
    const char* sql = "SELECT username, comment, createdAt FROM comments WHERE forumId = ? ORDER BY createdAt ASC;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_int(stmt, 1, forumId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Comment comment;
        comment.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        comment.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        comment.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        comments.push_back(comment);
    }

    sqlite3_finalize(stmt);
    return true;
}
