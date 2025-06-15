#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager()
{
    if (sqlite3_open("SimpleForum.db", &db))
    {
        LOGERROR("Failed to open SimpleForum.db: " + std::string(sqlite3_errmsg(db)));
        db = nullptr;
    } else
    {
        LOGINFO("Database connection established.");
    }
}

DatabaseManager::~DatabaseManager()
{
    closeConnection();
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

sqlite3* DatabaseManager::getConnection()
{
    return db;
}

void DatabaseManager::closeConnection()
{
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        LOGINFO("Database closed");
    }
}
