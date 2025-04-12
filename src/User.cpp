#include "../includes/User.h"
#include "../includes/DatabaseManager.h"
#include <vector>

User::User(int id, const std::string& username, const std::string& password,
    Role role, int crewBusId)
    : id(id), username(username), password(password), role(role), crewBusId(crewBusId) {}

bool User::authenticate(const std::string& password) const {
    return this->password == password;
}

User* User::authenticate(const std::string& username, const std::string& password) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, username, password, role, crewBusId FROM Users WHERE username = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string dbUsername = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string roleStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int crewBusId = sqlite3_column_int(stmt, 4);

        Role role = (roleStr == "admin") ? Role::Admin : Role::Crew;

        User* user = new User(id, dbUsername, dbPassword, role, crewBusId);

        if (user->authenticate(password)) {
            return user;
        };
        delete user;
    };

    sqlite3_finalize(stmt);
    return nullptr;
}