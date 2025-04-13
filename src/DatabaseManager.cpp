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

	"CREATE TABLE IF NOT EXISTS TOURIST_BUREAU_BUSES ("
	"id integer NOT NULL CONSTRAINT TOURIST_BUREAU_BUSES_pk PRIMARY KEY AUTOINCREMENT,"
	"number text NOT NULL,"
	"model text NOT NULL,"
	"totalMileage real NOT NULL,"
	"CONSTRAINT TOURIST_BUREAU_BUSES_ak_1 UNIQUE (number));"

	"CREATE TABLE IF NOT EXISTS TOURIST_BUREAU_CREW_MEMBERS ("
	"id integer NOT NULL CONSTRAINT TOURIST_BUREAU_CREW_MEMBERS_pk PRIMARY KEY AUTOINCREMENT,"
	"lastName text NOT NULL,"
	"employeeNumber text NOT NULL,"
	"experience integer NOT NULL,"
	"category text NOT NULL,"
	"address text NOT NULL,"
	"birthYear integer NOT NULL,"
	"busId integer NOT NULL,"
	"CONSTRAINT TOURIST_BUREAU_CREW_MEMBERS_ak_1 UNIQUE (employeeNumber),"
	"CONSTRAINT TOURIST_BUREAU_CREW_MEMBERS_TOURIST_BUREAU_BUSES FOREIGN KEY (busId)"
	"REFERENCES TOURIST_BUREAU_BUSES (id));"

	"CREATE TABLE IF NOT EXISTS TOURIST_BUREAU_CREW_PAYMENTS ("
	"id integer NOT NULL CONSTRAINT TOURIST_BUREAU_CREW_PAYMENTS_pk PRIMARY KEY AUTOINCREMENT,"
	"busId integer NOT NULL,"
	"amount real NOT NULL,"
	"paymentDate date NOT NULL,"
	"periodStart date NOT NULL,"
	"periodEnd date NOT NULL,"
	"CONSTRAINT TOURIST_BUREAU_BUSES_TOURIST_BUREAU_CREW_PAYMENTS FOREIGN KEY (busId)"
	"REFERENCES TOURIST_BUREAU_BUSES (id));"

	"CREATE TABLE IF NOT EXISTS TOURIST_BUREAU_ROUTES ("
	"id integer NOT NULL CONSTRAINT TOURIST_BUREAU_ROUTES_pk PRIMARY KEY AUTOINCREMENT,"
	"name text NOT NULL,"
	"startPoint text NOT NULL,"
	"endPoint text NOT NULL,"
	"distance real NOT NULL,"
	"CONSTRAINT TOURIST_BUREAU_ROUTES_ak_1 UNIQUE (name));"

	"CREATE TABLE IF NOT EXISTS TOURIST_BUREAU_TRIPS ("
	"id integer NOT NULL CONSTRAINT TOURIST_BUREAU_TRIPS_pk PRIMARY KEY AUTOINCREMENT,"
	"busId integer NOT NULL,"
	"departureDate date NOT NULL,"
	"arrivalDate date NOT NULL,"
	"routeName text NOT NULL,"
	"passengersCount integer NOT NULL,"
	"ticketPrice real NOT NULL,"
	"CONSTRAINT TOURIST_BUREAU_TRIPS_TOURIST_BUREAU_BUSES FOREIGN KEY (busId)"
	"REFERENCES TOURIST_BUREAU_BUSES (id),"
	"CONSTRAINT TOURIST_BUREAU_TRIPS_TOURIST_BUREAU_ROUTES FOREIGN KEY (routeName)"
	"REFERENCES TOURIST_BUREAU_ROUTES (name));"

	"CREATE TABLE IF NOT EXISTS TOURIST_BUREAU_USERS ("
	"id integer NOT NULL CONSTRAINT TOURIST_BUREAU_USERS_pk PRIMARY KEY AUTOINCREMENT,"
	"username text NOT NULL,"
	"password text NOT NULL,"
	"role text NOT NULL,"
	"crewMemberId integer,"
	"CONSTRAINT TOURIST_BUREAU_USERS_ak_1 UNIQUE (username),"
	"CONSTRAINT TOURIST_BUREAU_USERS_TOURIST_BUREAU_CREW_MEMBERS FOREIGN KEY (crewMemberId)"
	"REFERENCES TOURIST_BUREAU_CREW_MEMBERS (id));";

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