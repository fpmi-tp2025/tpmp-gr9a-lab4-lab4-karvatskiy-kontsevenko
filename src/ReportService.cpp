#include "../includes/ReportService.h"
#include "../includes/DatabaseManager.h"
#include "../includes/Bus.h"
#include "../includes/CrewMember.h"
#include <iomanip>
#include <sstream>

std::vector<TripReport> ReportService::getBusTrips(int busId, const std::string& startDate, const std::string& endDate) {
    std::vector<TripReport> reports;
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT departureDate, arrivalDate, routeName, passengersCount, ticketPrice "
        "FROM Trips WHERE busId = ? AND departureDate >= ? AND arrivalDate <= ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return reports;
    }

    sqlite3_bind_int(stmt, 1, busId);
    sqlite3_bind_text(stmt, 2, startDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, endDate.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TripReport report;
        report.departureDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        report.arrivalDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        report.routeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        report.passengersCount = sqlite3_column_int(stmt, 3);
        report.ticketPrice = sqlite3_column_double(stmt, 4);

        reports.push_back(report);
    }

    sqlite3_finalize(stmt);
    return reports;
}

double ReportService::calculateBusRevenue(int busId, const std::string& startDate, const std::string& endDate) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;
    double revenue = 0.0;

    const char* sql = "SELECT SUM(passengersCount * ticketPrice) "
        "FROM Trips WHERE busId = ? AND departureDate >= ? AND arrivalDate <= ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return revenue;
    }

    sqlite3_bind_int(stmt, 1, busId);
    sqlite3_bind_text(stmt, 2, startDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, endDate.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        revenue = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return revenue;
}

std::vector<CrewPayment> ReportService::calculateCrewPayments(double percentage, const std::string& startDate, const std::string& endDate) {
    std::vector<CrewPayment> payments;
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT busId, SUM(passengersCount * ticketPrice * ?) "
        "FROM Trips WHERE departureDate >= ? AND arrivalDate <= ? "
        "GROUP BY busId;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return payments;
    }

    sqlite3_bind_double(stmt, 1, percentage / 100.0);
    sqlite3_bind_text(stmt, 2, startDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, endDate.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        CrewPayment payment;
        payment.busId = sqlite3_column_int(stmt, 0);
        payment.amount = sqlite3_column_double(stmt, 1);

        std::ostringstream period;
        period << startDate << " - " << endDate;
        payment.period = period.str();

        payments.push_back(payment);
    }

    sqlite3_finalize(stmt);
    return payments;
}

void ReportService::saveCrewPayments(const std::vector<CrewPayment>& payments) {
    DatabaseManager& db = DatabaseManager::getInstance();

    for (const auto& payment : payments) {
        size_t sep = payment.period.find(" - ");
        std::string startDate = payment.period.substr(0, sep);
        std::string endDate = payment.period.substr(sep + 3);

        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO CrewPayments (busId, amount, paymentDate, periodStart, periodEnd) "
            "VALUES (?, ?, date('now'), ?, ?);";
        if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
            continue;
        }

        sqlite3_bind_int(stmt, 1, payment.busId);
        sqlite3_bind_double(stmt, 2, payment.amount);
        sqlite3_bind_text(stmt, 3, startDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, endDate.c_str(), -1, SQLITE_STATIC);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

double ReportService::getCrewPaymentByDate(int busId, const std::string& date) {
    DatabaseManager& db = DatabaseManager::getInstance();
    sqlite3_stmt* stmt;
    double amount = 0.0;

    const char* sql = "SELECT amount FROM CrewPayments WHERE busId = ? AND paymentDate = ?;";
    if (sqlite3_prepare_v2(db.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return amount;
    }

    sqlite3_bind_int(stmt, 1, busId);
    sqlite3_bind_text(stmt, 2, date.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        amount = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return amount;
}