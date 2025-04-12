#include "../includes/Trip.h"
#include "../includes/Bus.h"
#include "../includes/DatabaseManager.h"
#include <vector>

Trip::Trip(int id, int busId, const std::string& departureDate, const std::string& arrivalDate,
    const std::string& routeName, int passengersCount, double ticketPrice)
    : id(id), busId(busId), departureDate(departureDate), arrivalDate(arrivalDate),
    routeName(routeName), passengersCount(passengersCount), ticketPrice(ticketPrice) {}

Trip* Trip::create(int busId, const std::string& departureDate, const std::string& arrivalDate,
    const std::string& routeName, int passengersCount, double ticketPrice) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO Trips (busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice) "
        "VALUES (?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, busId);
    sqlite3_bind_text(stmt, 2, departureDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, arrivalDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, routeName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, passengersCount);
    sqlite3_bind_double(stmt, 6, ticketPrice);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    int id = sqlite3_last_insert_rowid(db.getDatabase());
    sqlite3_finalize(stmt);

    return new Trip(id, busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice);
}

std::vector<Trip*> Trip::getAll() {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<Trip*> trips;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice FROM Trips;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return trips;
    };

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int busId = sqlite3_column_int(stmt, 1);
        std::string departureDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string arrivalDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string routeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int passengersCount = sqlite3_column_int(stmt, 5);
        double ticketPrice = sqlite3_column_double(stmt, 6);

        trips.push_back(new Trip(id, busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice));
    };

    sqlite3_finalize(stmt);
    return trips;
}

std::vector<Trip*> Trip::findByBusId(int busId) {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<Trip*> trips;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, departureDate, arrivalDate, routeName, passengersCount, ticketPrice "
        "FROM Trips WHERE busId = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return trips;
    }

    sqlite3_bind_int(stmt, 1, busId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string departureDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string arrivalDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string routeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int passengersCount = sqlite3_column_int(stmt, 4);
        double ticketPrice = sqlite3_column_double(stmt, 5);

        trips.push_back(new Trip(id, busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice));
    }

    sqlite3_finalize(stmt);
    return trips;
}

std::vector<Trip*> Trip::findByDateRange(const std::string& startDate, const std::string& endDate) {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<Trip*> trips;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice "
        "FROM Trips WHERE departureDate >= ? AND arrivalDate <= ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return trips;
    }

    sqlite3_bind_text(stmt, 1, startDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, endDate.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int busId = sqlite3_column_int(stmt, 1);
        std::string departureDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string arrivalDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string routeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int passengersCount = sqlite3_column_int(stmt, 5);
        double ticketPrice = sqlite3_column_double(stmt, 6);

        trips.push_back(new Trip(id, busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice));
    }

    sqlite3_finalize(stmt);
    return trips;
}