#include "../includes/DatabaseManager.h"
#include <iostream>

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL,"
        "role TEXT NOT NULL,"
        "crewBusId INTEGER);"

        "CREATE TABLE IF NOT EXISTS Buses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "number TEXT NOT NULL UNIQUE,"
        "model TEXT NOT NULL,"
        "totalMileage REAL NOT NULL);"

        "CREATE TABLE IF NOT EXISTS CrewMembers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "lastName TEXT NOT NULL,"
        "employeeNumber TEXT NOT NULL UNIQUE,"
        "experience INTEGER NOT NULL,"
        "category TEXT NOT NULL,"
        "address TEXT NOT NULL,"
        "birthYear INTEGER NOT NULL,"
        "busId INTEGER NOT NULL,"
        "FOREIGN KEY(busId) REFERENCES Buses(id));"

        "CREATE TABLE IF NOT EXISTS Routes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "startPoint TEXT NOT NULL,"
        "endPoint TEXT NOT NULL,"
        "distance REAL NOT NULL);"

        "CREATE TABLE IF NOT EXISTS Trips ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "busId INTEGER NOT NULL,"
        "departureDate TEXT NOT NULL,"
        "arrivalDate TEXT NOT NULL,"
        "routeName TEXT NOT NULL,"
        "passengersCount INTEGER NOT NULL,"
        "ticketPrice REAL NOT NULL,"
        "FOREIGN KEY(busId) REFERENCES Buses(id));"

        "CREATE TABLE IF NOT EXISTS CrewPayments ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "busId INTEGER NOT NULL,"
        "amount REAL NOT NULL,"
        "paymentDate TEXT NOT NULL,"
        "periodStart TEXT NOT NULL,"
        "periodEnd TEXT NOT NULL,"
        "FOREIGN KEY(busId) REFERENCES Buses(id));";

    return execute(sql);
}

void DatabaseManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::executeWithParams(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
    }

    bool result = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return result;
}