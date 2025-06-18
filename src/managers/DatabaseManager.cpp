#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() {}

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::ScopedConnection::ScopedConnection() {
    if (sqlite3_open("SimpleForum.db", &db) != SQLITE_OK) {
        LOGERROR("Failed to open DB: " + std::string(sqlite3_errmsg(db)));
        db = nullptr;
    }
    else {
        LOGINFO("Database connection opened.");
    }
}

DatabaseManager::ScopedConnection::~ScopedConnection() {
    if (db) {
        sqlite3_close(db);
        LOGINFO("Database connection closed.");
    }
}

sqlite3* DatabaseManager::ScopedConnection::get() {
    return db;
}

bool DatabaseManager::ScopedConnection::isValid() const {
    return db != nullptr;
}
