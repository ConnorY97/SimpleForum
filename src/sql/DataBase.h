#pragma once
#include <sqlite3.h>

class DataBase
{
public:
    DataBase();
    ~DataBase();

    bool registerUser(const std::string& username, const std::string& password);

private:
    sqlite3* dataBase = nullptr;
}