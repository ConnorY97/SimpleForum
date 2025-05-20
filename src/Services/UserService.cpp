#include "UserService.h"

UserService::UserService()
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

    std::cout << "Initiated sql database" << std::endl;
}

UserService::~UserService()
{
    if (dataBase != nullptr)
    {
        sqlite3_close(dataBase);
    }
}

bool UserService::registerUser(const std::string& username, const std::string& password, std::string& error)
{
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = std::string("Prepare failed: ") + sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = std::string("Register failed: ") + sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool UserService::loginUser (const std::string&username, const std::string& password, std::string& error)
{
    if (username.empty() || password.empty())
    {
        error = "Missing username or password";
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    auto user = users_.find(username);
    if (user == users_.end())
    {
        error = "Username does not exist";
        return false;
    }

    if (user->second != password)
    {
        error = "Password is incorrect";
        return false;
    }
    return true;
}