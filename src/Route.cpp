#include "../includes/Route.h"
#include "../includes/Trip.h"
#include "../includes/DatabaseManager.h"
#include <vector>

Route::Route(int id, const std::string& name, const std::string& startPoint,
    const std::string& endPoint, double distance)
    : id(id), name(name), startPoint(startPoint), endPoint(endPoint), distance(distance) {}

Route* Route::create(const std::string& name, const std::string& startPoint,
    const std::string& endPoint, double distance) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO TOURIST_BUREAU_ROUTES (name, startPoint, endPoint, distance) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, startPoint.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, endPoint.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, distance);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return nullptr;
    };

    int id = sqlite3_last_insert_rowid(db.getDatabase());
    sqlite3_finalize(stmt);

    return new Route(id, name, startPoint, endPoint, distance);
}

bool Route::update(const std::string& name, const std::string& startPoint,
    const std::string& endPoint, double distance) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "UPDATE TOURIST_BUREAU_ROUTES SET name = ?, startPoint = ?, endPoint = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    };

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, startPoint.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, endPoint.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, distance);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    };
    sqlite3_finalize(stmt);

    return true;
}

bool Route::remove() {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "DELETE FROM TOURIST_BUREAU_ROUTES WHERE id = ?;";
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

Route* Route::findById(int id) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT name, startPoint, endPoint, distance FROM TOURIST_BUREAU_ROUTES WHERE id = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    };

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string startPoint = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string endPoint = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double distance = sqlite3_column_double(stmt, 3);

        sqlite3_finalize(stmt);
        return new Route(id, name, startPoint, endPoint, distance);
    };

    sqlite3_finalize(stmt);
    return nullptr;
}

std::vector<Route*> Route::getAll() {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<Route*> routes;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, name, startPoint, endPoint, distance FROM TOURIST_BUREAU_ROUTES;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return routes;
    };

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string startPoint = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string endPoint = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double distance = sqlite3_column_double(stmt, 4);

        routes.push_back(new Route(id, name, startPoint, endPoint, distance));
    };

    sqlite3_finalize(stmt);
    return routes;
}

std::vector<Trip*> Route::getTrips() const {
    DatabaseManager& db = DatabaseManager::getInstance();
    std::vector<Trip*> trips;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, busId, departureDate, arrivalDate, passengersCount, ticketPrice "
        "FROM TOURIST_BUREAU_ROUTES WHERE routeName = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return trips;
    };

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int busId = sqlite3_column_int(stmt, 1);
        std::string departureDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string arrivalDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int passengersCount = sqlite3_column_int(stmt, 4);
        double ticketPrice = sqlite3_column_double(stmt, 5);

        trips.push_back(new Trip(id, busId, departureDate, arrivalDate, name, passengersCount, ticketPrice));
    };

    sqlite3_finalize(stmt);
    return trips;
}