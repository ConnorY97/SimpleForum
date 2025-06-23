#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "../utils/LoggerUtilities.h"
#include <sqlite3.h>
#include <string>

class DatabaseManager
{
  public:
    static DatabaseManager &getInstance();

    // Use this in services to get a scoped DB connection
    class ScopedConnection
    {
      public:
        ScopedConnection();
        ~ScopedConnection();

        sqlite3 *get();
        bool isValid() const;

      private:
        sqlite3 *db;
    };

  private:
    DatabaseManager();
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;
};

#endif // DATABASE_MANAGER_H
