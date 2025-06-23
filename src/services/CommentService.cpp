#include "CommentService.h"

// Constructor: Initializes the SQLite database connection and creates the 'comments' table if it
// doesn't exist.
CommentService::CommentService()
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        LOGERROR("DB connection failed");
        return;
    }

    sqlite3 *dataBase = conn.get();

    // SQL command to create the 'comments' table with necessary fields and a foreign key reference
    // to 'forums'.
    const char *create_comments_sql =
        "CREATE TABLE IF NOT EXISTS comments ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "         // Unique ID for each comment
        "forumId INTEGER NOT NULL, "                     // Associated forum's ID (foreign key)
        "username TEXT NOT NULL, "                       // Username of the comment author
        "comment TEXT NOT NULL, "                        // Actual comment text
        "createdAt DATETIME DEFAULT CURRENT_TIMESTAMP, " // Auto-generated creation timestamp
        "FOREIGN KEY(forumId) REFERENCES forums(id) ON DELETE CASCADE);"; // Delete comments if
                                                                          // forum is deleted

    char *errMsg = nullptr;

    // Execute the table creation SQL
    if (sqlite3_exec(dataBase, create_comments_sql, 0, 0, &errMsg) != SQLITE_OK)
    {
        LOGERROR("Failed to create comments table: " + std::string(errMsg));
        sqlite3_free(errMsg);
    }

    LOGINFO("Comments Database Establised");
}

// Adds a new comment to the database for a given forum.
bool CommentService::addComment(int forumId, const std::string &username,
                                const std::string &commentText, int &commentId, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "INSERT INTO comments (forumId, username, comment, createdAt) VALUES (?, ?, "
                      "?, datetime('now'));";

    sqlite3_stmt *stmt;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind values to SQL parameters
    sqlite3_bind_int(stmt, 1, forumId);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, commentText.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement and check for success
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    // Retrieve the ID of the newly inserted comment
    commentId = static_cast<int>(sqlite3_last_insert_rowid(dataBase));

    sqlite3_finalize(stmt);
    return true;
}

// Retrieves all comments for a specific forum ID, sorted by creation time.
bool CommentService::getCommentsForForum(int forumId, std::vector<Comment> &comments,
                                         std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "SELECT id, username, comment, createdAt FROM comments WHERE forumId = ? "
                      "ORDER BY createdAt ASC;";
    sqlite3_stmt *stmt;

    // Prepare the SQL query
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind the forum ID
    sqlite3_bind_int(stmt, 1, forumId);

    // Loop through the results and populate the comments vector
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Comment comment;
        comment.id = sqlite3_column_int(stmt, 0);
        comment.username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        comment.comment = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        comment.createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        comments.push_back(comment);
    }

    sqlite3_finalize(stmt);
    return true;
}

// Fetches a single comment by its ID.
bool CommentService::getCommentById(int commentId, Comment &comment, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql =
        "SELECT id, forumID, username, comment, createdAt FROM comments WHERE id = ?;";
    sqlite3_stmt *stmt;

    // Prepare SQL query
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind the comment ID
    sqlite3_bind_int(stmt, 1, commentId);

    // Retrieve and store the comment data
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        comment.id = sqlite3_column_int(stmt, 0);
        comment.forumId = sqlite3_column_int(stmt, 1);
        comment.username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        comment.comment = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        comment.createdAt = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        error = "Comment not found";
        sqlite3_finalize(stmt);
        return false;
    }
}

// Updates the text of a comment if the username matches.
bool CommentService::updateCommentById(int commentId, std::string &username,
                                       const std::string &newText, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql;
    sqlite3_stmt *stmt;

    // Admin can bypass the username check
    if (username == "Admin")
    {
        sql = "UPDATE comments SET comment = ? WHERE id = ?;";
    }
    else
    {
        sql = "UPDATE comments SET comment = ? WHERE id = ? AND username = ?;";
    }

    // Prepare the SQL query
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind parameters: new text, comment ID, and username for verification
    sqlite3_bind_text(stmt, 1, newText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, commentId);
    sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the update
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    // Check if any rows were actually updated
    int changes = sqlite3_changes(dataBase);
    sqlite3_finalize(stmt);

    if (changes == 0)
    {
        error = "No comment updated. You may not be the owner or the comment does not exist.";
        return false;
    }

    return true;
}

// Deletes a comment by its ID.
bool CommentService::deleteCommentById(int commentId, std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "DELETE FROM comments WHERE id = ?;";
    sqlite3_stmt *stmt;

    // Prepare the SQL delete query
    if (sqlite3_prepare_v2(dataBase, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        error = sqlite3_errmsg(dataBase);
        return false;
    }

    // Bind the comment ID
    sqlite3_bind_int(stmt, 1, commentId);

    // Execute the deletion
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        error = sqlite3_errmsg(dataBase);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Deletes all comments from the database (use with caution).
bool CommentService::clearComments(std::string &error)
{
    DatabaseManager::ScopedConnection conn;
    if (!conn.isValid())
    {
        error = "Failed to connect to database";
        return false;
    }

    sqlite3 *dataBase = conn.get();

    const char *sql = "DELETE FROM comments"; // Deletes all records in the table
    char *errMsg = nullptr;

    // Execute the SQL command
    if (sqlite3_exec(dataBase, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        error = std::string("Failed to clear comments: ") + errMsg;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
