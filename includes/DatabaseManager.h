#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "sqlite/sqlite3.h"
#include <string>
#include <vector>

class DatabaseManager {
public:
    static DatabaseManager& getInstance();

    bool initialize(const std::string& dbPath);
    void close();

    sqlite3* getDatabase() const { return db; }

    bool execute(const std::string& sql);
    bool executeWithParams(const std::string& sql, const std::vector<std::string>& params);

private:
    DatabaseManager() : db(nullptr) {}
    ~DatabaseManager() { close(); }

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    sqlite3* db;
};

#endif