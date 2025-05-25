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

    std::cout << "Initialized SQL database" << std::endl;
}

ForumService::~ForumService()
{
    if (dataBase) {
        sqlite3_close(dataBase);
    }
}

bool ForumService::createForum(const std::string& title, const std::string& description, const std::string& createdBy, std::string& error)
{
    const char* sql =
        "INSERT INTO forums (title, description, createdBy, createdAt) "
        "VALUES (?, ?, ?, datetime('now'));";

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

    sqlite3_finalize(stmt);
    return true;
}

std::vector<Forum> ForumService::listForums(std::string& error)
{
    std::vector<Forum> forums;

    const char* sql = "SELECT id, title, description, createdBy, createdAt FROM forums;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(dataBase);
        return forums;
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
    return forums;
}
