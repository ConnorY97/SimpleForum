#include "DataBase.h"
#include <iostream>

DataBase::DataBase()
{
    if (sqlite3_open("forum.db", &dataBase)) {
        std::cerr << "Can't open DB: " << sqlite3_errmsg(dataBase) << std::endl;
    }

    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE, "
        "password TEXT);";

    char* errMsg = nullptr;
    if (sqlite3_exec(dataBase, create_table_sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
};

DataBase::~DataBase()
{
    if (dataBase != nullptr)
    {
        sqlite3_close(dataBase);
    }
};

bool DataBase::registerUser(const std::string& username, const std::string& password)
{
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(dataBase) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(dataBase) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
