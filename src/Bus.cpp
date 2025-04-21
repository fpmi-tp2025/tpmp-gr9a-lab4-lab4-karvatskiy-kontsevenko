#include "../includes/Bus.h"
#include "../includes/CrewMember.h"
#include "../includes/DatabaseManager.h"
#include <vector>

Bus::Bus(int id, const std::string& number, const std::string& model, double totalMileage)
    : id(id), number(number), model(model), totalMileage(totalMileage) {}

Bus* Bus::create(const std::string& number, const std::string& model, double totalMileage) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO TOURIST_BUREAU_BUSES (number, model, totalMileage) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    int id = sqlite3_last_insert_rowid(db.getDatabase());

    sqlite3_bind_text(stmt, 1, number.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, model.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, totalMileage);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return nullptr;
    };
    sqlite3_finalize(stmt);

    return new Bus(id, number, model, totalMileage);
}

bool Bus::update(const std::string& number, const std::string& model, double totalMileage) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "UPDATE TOURIST_BUREAU_BUSES SET number = ?, model = ?, totalMileage = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    };

    sqlite3_bind_text(stmt, 1, number.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, model.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, totalMileage);
    sqlite3_bind_int(stmt, 4, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    };
    sqlite3_finalize(stmt);

    return true;
}

bool Bus::remove() {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "DELETE FROM TOURIST_BUREAU_BUSES WHERE id = ?;";
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

Bus* Bus::findById(int id) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT number, model, totalMileage FROM TOURIST_BUREAU_BUSES WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double totalMileage = sqlite3_column_double(stmt, 2);

        sqlite3_finalize(stmt);
        return new Bus(id, number, model, totalMileage);
    };

    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<Bus*> Bus::getAll() {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<Bus*> buses;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, number, model, totalMileage FROM TOURIST_BUREAU_BUSES;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return buses;
    };

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double totalMileage = sqlite3_column_double(stmt, 3);

        buses.push_back(new Bus(id, number, model, totalMileage));
    };

    sqlite3_finalize(stmt);
    return buses;
}

std::vector<CrewMember*> Bus::getCrewMembers() const {
    return CrewMember::findByBusId(id);
}