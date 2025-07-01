#include "ForumService.h"

// Constructor - Opens the SQLite database and ensures the forums table exists
ForumService::ForumService()
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        LOGERROR("DB connection failed");
        return;
    }

    sqlite3 *dataBase = conn.get();

    // SQL statement to create the forums table if it doesn't already exist
    const char *create_forums_sql =
        "CREATE TABLE IF NOT EXISTS forums ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, " // Unique ID for each forum
        "title TEXT NOT NULL, "                  // Forum title (required)
        "description TEXT, "                     // Optional description
        "createdBy TEXT NOT NULL, "              // Creator's username (required)
        "createdAt TEXT NOT NULL);";             // Timestamp of forum creation

    char *errMsg = nullptr;

    // Execute SQL statement to create the table
    if (sqlite3_exec(dataBase, create_forums_sql, 0, 0, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Failed to create forums table: " << errMsg << std::endl;
        sqlite3_free(errMsg); // Free the error message memory
    }

    LOGINFO("Forums Database Establised");
}

// Creates a new forum and returns its ID
bool ForumService::createForum(const std::string &title, const std::string &description,
                               const std::string &createdBy, int &forumId, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    // SQL insert statement with datetime now
    const char *sql = "INSERT INTO forums (title, description, createdBy, createdAt) VALUES (?, ?, "
                      "?, datetime('now'));";

    sqlite3_stmt *stmt;
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind parameters to the SQL statement
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, createdBy.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    // Retrieve the ID of the newly inserted forum
    forumId = static_cast<int>(sqlite3_last_insert_rowid(dataBase));
    sqlite3_finalize(stmt);
    return true;
}

// Updates a forum if the requesting user is the creator
bool ForumService::updateForumById(int forumId, std::string username, const std::string &newTitle,
                                   const std::string &newDescription, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        LOGERROR("DB connection failed");
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql =
        "UPDATE forums SET title = ?, description = ? WHERE id = ? AND createdBy = ?;";
    sqlite3_stmt *stmt;

    // Prepare the update statement
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind the parameters: new title, new description, forum ID, and username
    sqlite3_bind_text(stmt, 1, newTitle.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, newDescription.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, forumId);
    sqlite3_bind_text(stmt, 4, username.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the update
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    // Check if any row was actually updated
    int changes = sqlite3_changes(dataBase);
    sqlite3_finalize(stmt);

    if (changes == 0)
    {
        error = "No forum updated. You may not be the owner or the forum does not exist.";
        return false;
    }

    return true;
}

// Retrieves all forums and stores them in the provided vector
bool ForumService::listForums(std::vector<Forum> &forums, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "SELECT id, title, description, createdBy, createdAt FROM forums;";
    sqlite3_stmt *stmt;

    // Prepare the select statement
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Loop through each row in the result set
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Forum forum;
        forum.id = sqlite3_column_int(stmt, 0);
        forum.title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        forum.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        forum.createdBy = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        forum.createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        forums.push_back(forum);
    }

    sqlite3_finalize(stmt);
    return true;
}

// Retrieves a specific forum by ID
bool ForumService::getForumById(int id, Forum &forum, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        LOGERROR("DB connection failed");
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql =
        "SELECT id, title, description, createdBy, createdAt FROM forums WHERE id = ?;";
    sqlite3_stmt *stmt;

    // Prepare the select statement
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind the forum ID
    sqlite3_bind_int(stmt, 1, id);

    // Attempt to step through a single result row
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        forum.id = sqlite3_column_int(stmt, 0);
        forum.title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        forum.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        forum.createdBy = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        forum.createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        sqlite3_finalize(stmt);
        return true;
    }

    // No result found
    sqlite3_finalize(stmt);
    error = "Forum not found";
    return false;
}

bool ForumService::deleteForumById(int id, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *database = conn.get();

    const char *sql = "DELETE FROM forums WHERE id = ?;";
    sqlite3_stmt *stmt;

    // Prepare the SQL delete query
    if (sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(database);
        return false;
    }

    // Bind the forum ID
    sqlite3_bind_int(stmt, 1, id);

    // Execute the deletion
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(database);
        sqlite3_finalize(stmt);
        return false;
    }

    int changes = sqlite3_changes(database);
    sqlite3_finalize(stmt);

    if (changes == 0)
    {
        error = "Forum not found or already deleted.";
        return false;
    }

    return true;
}

// Deletes all forums from the database (useful for testing or admin tools)
bool ForumService::clearForums(std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        LOGERROR("DB connection failed");
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "DELETE FROM forums";
    char *errMsg = nullptr;

    // Execute the delete statement
    if (sqlite3_exec(dataBase, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        error = std::string("Failed to clear forums: ") + errMsg;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
