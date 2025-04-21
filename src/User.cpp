#include "../includes/User.h"
#include "../includes/DatabaseManager.h"
#include <vector>

User::User(int id, const std::string& username, const std::string& password,
    Role role, int crewMemberId)
    : id(id), username(username), password(password), role(role), crewMemberId(crewMemberId) {}

User* User::create(const std::string& username, const std::string& password, const std::string& role, int crewMemberId) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO TOURIST_BUREAU_USERS (username, password, role, crewMemberId) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    int id = sqlite3_last_insert_rowid(db.getDatabase());

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, crewMemberId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return nullptr;
    };
    sqlite3_finalize(stmt);

    return new User(id, username, password, (role == "admin") ? Role::Admin : Role::Crew, crewMemberId);
}

bool User::authenticate(const std::string& password) const {
    return this->password == password;
}

User* User::authenticate(const std::string& username, const std::string& password) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, username, password, role, crewMemberId FROM TOURIST_BUREAU_USERS WHERE username = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string dbUsername = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string roleStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int crewMemberId = sqlite3_column_int(stmt, 4);

        Role role = (roleStr == "admin") ? Role::Admin : Role::Crew;

        User* user = new User(id, dbUsername, dbPassword, role, crewMemberId);

        if (user->authenticate(password)) {
            return user;
        };
        delete user;
    };

    sqlite3_finalize(stmt);
    return nullptr;
}