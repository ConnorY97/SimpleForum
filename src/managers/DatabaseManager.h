#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include "../utils/LoggerUtilities.h"

class DatabaseManager
{
public:
    static DatabaseManager& getInstance();
    sqlite3* getConnection();
    void closeConnection();

private:
    DatabaseManager(); // private constructor
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    sqlite3* db;
};

#endif // DATABASE_MANAGER_H
