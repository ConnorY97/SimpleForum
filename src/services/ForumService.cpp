#include "ForumService.h"

ForumService::ForumService()
{
    if (sqlite3_open("forum.db", &dataBase)) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(dataBase) << std::endl;
    }

    const char* create_forums_sql =
        "CREATE TABLE IF NOT EXISTS forums ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "description TEXT, "
        "createdBy TEXT NOT NULL, "
        "createdAt TEXT NOT NULL);";

    char* errMsg = nullptr;

    if (sqlite3_exec(dataBase, create_forums_sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to create forums table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    std::cout << "Initialized Forum Database" << std::endl;
}

ForumService::~ForumService()
{
    if (dataBase) {
        sqlite3_close(dataBase);
    }
}

bool ForumService::createForum(const std::string& title, const std::string& description, const std::string& createdBy, int& forumId, std::string& error)
{
    const char* sql =
        "INSERT INTO forums (title, description, createdBy, createdAt) VALUES (?, ?, ?, datetime('now'));";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, createdBy.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    forumId = static_cast<int>(sqlite3_last_insert_rowid(dataBase));
    sqlite3_finalize(stmt);
    return true;
}

bool ForumService::listForums(std::vector<Forum>& forums, std::string& error)
{
    const char* sql = "SELECT id, title, description, createdBy, createdAt FROM forums;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Forum forum;
        forum.id = sqlite3_column_int(stmt, 0);
        forum.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        forum.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        forum.createdBy = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        forum.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        forums.push_back(forum);
    }

    sqlite3_finalize(stmt);
    return true;
}


bool ForumService::getForumById(int id, Forum& forum, std::string& error)
{
    const char* sql = "SELECT id, title, description, createdBy, createdAt FROM forums WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        forum.id = sqlite3_column_int(stmt, 0);
        forum.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        forum.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        forum.createdBy = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        forum.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    error = "Forum not found";
    return false;
}

bool ForumService::clearForums(std::string& error)
{
    const char* sql = "DELETE FROM forums";
    char* errMsg = nullptr;

    if (sqlite3_exec(dataBase, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        error = std::string("Failed to clear forums: ") + errMsg;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

