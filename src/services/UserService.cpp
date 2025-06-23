#include "UserService.h"

UserService::UserService()
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        LOGERROR("DB connection failed");
        return;
    }

    sqlite3 *dataBase = conn.get();

    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS users ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "username TEXT UNIQUE, "
                                   "password TEXT);";

    char *errMsg = nullptr;
    if (sqlite3_exec(dataBase, create_table_sql, 0, 0, &errMsg) != SQLITE_OK)
    {
        LOGERROR("Failed to create table with error: " + std::string(errMsg));
        sqlite3_free(errMsg);
    }

    LOGINFO("User Database Established");
}

bool UserService::registerUser(const std::string &username, const std::string &password,
                               std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = std::string("Prepare failed: ") + sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = std::string("Register failed: ") + sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool UserService::registerUser(const std::string &username, const std::string &password,
                               std::string &confirmPassword, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    if (password != confirmPassword)
    {
        error = "Passwords do not match!";
        return false;
    }

    const char *sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = std::string("Prepare failed: ") + sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = std::string("Register failed: ") + sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool UserService::loginUser(const std::string &username, const std::string &password,
                            std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    if (username.empty() || password.empty())
    {
        error = "Missing username or password";
        return false;
    }

    const char *sql = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = std::string("Prepare failed: ") + sqlite3_errmsg(dataBase);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *dbPassword = sqlite3_column_text(stmt, 0);
        if (dbPassword && password == reinterpret_cast<const char *>(dbPassword))
        {
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            error = "Password is incorrect";
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else if (rc == SQLITE_DONE)
    {
        error = "Username does not exist";
    }
    else
    {
        error = std::string("Query failed: ") + sqlite3_errmsg(dataBase);
    }

    sqlite3_finalize(stmt);
    return false;
}

bool UserService::ListUsers(std::vector<User> &users, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *database = conn.get();

    const char *sql = "SELECT id, username, password FROM users";
    sqlite3_stmt *stmt;

    // Prepare the selected statement
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(database);
        return false;
    }

    // Loop through each row in the result set
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User user;
        user.id = sqlite3_column_int(stmt, 0);
        user.username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        user.password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        users.push_back(user);
    }

    sqlite3_finalize(stmt);
    return true;
}

bool UserService::clearUsers(std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "DELETE FROM users;";
    char *errMsg = nullptr;

    if (sqlite3_exec(dataBase, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        error = std::string("Failed to clear users: ") + errMsg;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
