#include "../includes/CrewMember.h"
#include "../includes/Bus.h"
#include "../includes/DatabaseManager.h"
#include <vector>

CrewMember::CrewMember(int id, const std::string& lastName, const std::string& employeeNumber,
    int experience, const std::string& category, const std::string& address,
    int birthYear, int busId)
    : id(id), lastName(lastName), employeeNumber(employeeNumber), experience(experience),
    category(category), address(address), birthYear(birthYear), busId(busId) {}

CrewMember* CrewMember::create(const std::string& lastName, const std::string& employeeNumber,
    int experience, const std::string& category, const std::string& address,
    int birthYear, int busId) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO TOURIST_BUREAU_CREW_MEMBERS (lastName, employeeNumber, experience, category, address, birthYear, busId) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, employeeNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, experience);
    sqlite3_bind_text(stmt, 4, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, birthYear);
    sqlite3_bind_int(stmt, 7, busId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    int id = sqlite3_last_insert_rowid(db.getDatabase());
    sqlite3_finalize(stmt);

    return new CrewMember(id, lastName, employeeNumber, experience, category, address, birthYear, busId);
}

bool CrewMember::update(const std::string& lastName, const std::string& employeeNumber,
    int experience, const std::string& category, const std::string& address,
    int birthYear, int busId) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "UPDATE TOURIST_BUREAU_CREW_MEMBERS SET lastName = ?, employeeNumber = ?,"
        "experience = ?, category = ?, address = ?,"
        "birthYear = ?, busId = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    };

    sqlite3_bind_text(stmt, 1, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, employeeNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, experience);
    sqlite3_bind_text(stmt, 4, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, birthYear);
    sqlite3_bind_int(stmt, 7, busId);
    sqlite3_bind_int(stmt, 8, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    };
    sqlite3_finalize(stmt);

    return true;
}

bool CrewMember::remove() {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "DELETE FROM TOURIST_BUREAU_CREW_MEMBERS WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    };

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    };
    sqlite3_finalize(stmt);

    return true;
}

CrewMember* CrewMember::findById(int id) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT lastName, employeeNumber, experience, category, address, birthYear, busId "
        "FROM TOURIST_BUREAU_CREW_MEMBERS WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string employeeNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int experience = sqlite3_column_int(stmt, 2);
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int birthYear = sqlite3_column_int(stmt, 5);
        int busId = sqlite3_column_int(stmt, 6);

        sqlite3_finalize(stmt);
        return new CrewMember(id, lastName, employeeNumber, experience, category, address, birthYear, busId);
    }

    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<CrewMember*> CrewMember::findByBusId(int busId) {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<CrewMember*> members;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, lastName, employeeNumber, experience, category, address, birthYear "
        "FROM TOURIST_BUREAU_CREW_MEMBERS WHERE busId = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return members;
    }

    sqlite3_bind_int(stmt, 1, busId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string employeeNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int experience = sqlite3_column_int(stmt, 3);
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int birthYear = sqlite3_column_int(stmt, 6);

        members.push_back(new CrewMember(id, lastName, employeeNumber, experience,
            category, address, birthYear, busId));
    }

    sqlite3_finalize(stmt);
    return members;
}

std::vector<CrewMember*> CrewMember::getAll() {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<CrewMember*> members;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, lastName, employeeNumber, experience, category, address, birthYear, busId FROM TOURIST_BUREAU_CREW_MEMBERS;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return members;
    };

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string employeeNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int experience = sqlite3_column_int(stmt, 3);
        std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int birthYear = sqlite3_column_int(stmt, 6);
        int busId = sqlite3_column_int(stmt, 7);

        members.push_back(new CrewMember(id, lastName, employeeNumber, experience, category, address, birthYear, busId));
    };

    sqlite3_finalize(stmt);
    return members;
}