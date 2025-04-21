#include "../includes/DatabaseManager.h"
#include "../includes/User.h"
#include "../includes/Bus.h"
#include "../includes/CrewMember.h"
#include "../includes/Route.h"
#include "../includes/Trip.h"
#include "../includes/ReportService.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

void printBusInfo(const Bus* bus) {
    if (bus) {
        cout << "Автобус #" << bus->getId() << endl;
        cout << "Номер: " << bus->getNumber() << endl;
        cout << "Модель: " << bus->getModel() << endl;
        cout << "Пробег: " << bus->getTotalMileage() << " км" << endl;
    }
    else {
        cout << "Автобус не найден!" << endl;
    };
}

void demoUsers() {
    printSeparator();
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ С ПОЛЬЗОВАТЕЛЯМИ" << endl;
    printSeparator();

    User* admin = User::create("admin", "12345", "admin", -1);
    if (admin) {
        cout << "Создан новый пользователь: " << endl;
        cout << "Имя пользователя: " << admin->getUsername() << endl;
        cout << "Роль: " << ((admin->getRole() == User::Role::Admin) ? "Администратор" : "член экипажа") << endl;
    };

    User* validUser = User::authenticate("admin", "12345");
    if (admin) {
        cout << "Успешная аутентификация администратора: " << validUser->getUsername() << endl;
        delete validUser;
    }
    else {
        cout << "Ошибка аутентификации администратора!" << endl;
    };

    User* invalidUser = User::authenticate("admin", "admin");
    if (!invalidUser) {
        cout << "Неудачная аутентификация (как и ожидалось)" << endl;
    };

    printSeparator();
}

void demoBuses() {
    printSeparator();
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ С АВТОБУСАМИ" << endl;
    printSeparator();

    Bus* newBus = Bus::create("AB1234", "Mercedes Tourismo", 15000.5);
    if (newBus) {
        cout << "Создан новый автобус:" << endl;
        printBusInfo(newBus);

        Bus* foundBus = Bus::findById(newBus->getId());
        if (foundBus) {
            cout << "\nНайден автобус по ID:" << endl;
            printBusInfo(foundBus);
            delete foundBus;
        };

        delete newBus;
    };

    vector<Bus*> buses = Bus::getAll();
    cout << "\nВсе автобусы в системе (" << buses.size() << "):" << endl;
    for (const auto& bus : buses) {
        printBusInfo(bus);
        delete bus;
    };

    printSeparator();
}

void demoCrewMembers() {
    printSeparator();
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ С ЭКИПАЖАМИ" << endl;
    printSeparator();

    Bus* busForCrew = Bus::create("XY5678", "MAN Lion's Coach", 20000.0);
    if (!busForCrew) {
        cout << "Не удалось создать автобус для демонстрации экипажа" << endl;
        return;
    };

    CrewMember* newMember = CrewMember::create(
        "Иванов", "EMP001", 5, "Первая", "ул. Ленина, 10", 1985, busForCrew->getId()
    );

    if (newMember) {
        cout << "Создан новый член экипажа: " << newMember->getLastName() << endl;

        Bus* memberBus = Bus::findById(newMember->getBusId());
        if (memberBus) {
            cout << "Работает на автобусе: " << memberBus->getNumber() << endl;
            delete memberBus;
        }

        delete newMember;
    };

    vector<CrewMember*> crew = CrewMember::findByBusId(busForCrew->getId());
    cout << "\nЭкипаж автобуса " << busForCrew->getNumber() << " (" << crew.size() << " человек):" << endl;
    for (const auto& member : crew) {
        cout << "- " << member->getLastName() << endl;
        delete member;
    };

    delete busForCrew;
    printSeparator();
}

void demoRoutes() {
    printSeparator();
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ С МАРШРУТАМИ" << endl;
    printSeparator();

    Route* newRoute = Route::create(
        "Золотое кольцо", "Москва", "Суздаль", 250.5
    );

    if (newRoute) {
        cout << "Создан новый маршрут: " << newRoute->getName() << endl;
        cout << "От " << newRoute->getStartPoint() << " до " << newRoute->getEndPoint() << endl;
        cout << "Расстояние: " << newRoute->getDistance() << " км" << endl;

        delete newRoute;
    };

    vector<Route*> routes = Route::getAll();
    cout << "\nВсе маршруты в системе (" << routes.size() << "):" << endl;
    for (const auto& route : routes) {
        cout << "- " << route->getName() << " (" << route->getDistance() << " км)" << endl;
        delete route;
    };

    printSeparator();
}

void demoTrips() {
    printSeparator();
    cout << "ДЕМОНСТРАЦИЯ РАБОТЫ С ПОЕЗДКАМИ" << endl;
    printSeparator();

    Bus* tripBus = Bus::create("CD9012", "Setra S 515 HD", 18000.0);
    if (!tripBus) {
        cout << "Не удалось создать автобус для демонстрации поездки" << endl;
        return;
    };

    Trip* newTrip = Trip::create(
        tripBus->getId(), "2023-06-01", "2023-06-03",
        "Золотое кольцо", 30, 5000.0
    );

    if (newTrip) {
        cout << "Создана новая поездка:" << endl;
        cout << "Даты: " << newTrip->getDepartureDate() << " - " << newTrip->getArrivalDate() << endl;
        cout << "Маршрут: " << newTrip->getRouteName() << endl;
        cout << "Пассажиров: " << newTrip->getPassengersCount() << endl;
        cout << "Цена билета: " << newTrip->getTicketPrice() << " руб." << endl;

        Bus* tripBusInfo = Bus::findById(newTrip->getBusId());
        if (tripBusInfo) {
            cout << "Автобус: " << tripBusInfo->getNumber() << endl;
            delete tripBusInfo;
        };

        delete newTrip;
    };

    delete tripBus;
    printSeparator();
}

void demoReports() {
    printSeparator();
    cout << "ДЕМОНСТРАЦИЯ ОТЧЕТОВ" << endl;
    printSeparator();

    Bus* reportBus = Bus::create("EF3456", "Neoplan Tourliner", 25000.0);
    if (!reportBus) {
        cout << "Не удалось создать автобус для демонстрации отчётов" << endl;
        return;
    };

    Trip::create(reportBus->getId(), "2023-05-10", "2023-05-12", "Золотое кольцо", 25, 4500.0);
    Trip::create(reportBus->getId(), "2023-05-15", "2023-05-17", "Серебряное кольцо", 30, 5500.0);
    Trip::create(reportBus->getId(), "2023-06-01", "2023-06-05", "Золотое кольцо", 28, 5000.0);

    cout << "1. Рейсы автобуса за май 2023:" << endl;
    vector<TripReport> trips = ReportService::getBusTrips(
        reportBus->getId(), "2023-05-01", "2023-05-31"
    );

    for (const auto& trip : trips) {
        cout << "- " << trip.routeName << " (" << trip.departureDate << " - " << trip.arrivalDate << ")" << endl;
        cout << "  Пассажиров: " << trip.passengersCount << ", Цена: " << trip.ticketPrice << " руб." << endl;
    };

    cout << "\n2. Общая выручка автобуса:" << endl;
    double revenue = ReportService::calculateBusRevenue(
        reportBus->getId(), "2023-05-01", "2023-05-31"
    );
    cout << "Выручка: " << revenue << " руб." << endl;

    cout << "\n3. Начисления экипажам (20% от выручки):" << endl;
    vector<CrewPayment> payments = ReportService::calculateCrewPayments(
        20.0, "2023-05-01", "2023-05-31"
    );

    for (const auto& payment : payments) {
        cout << "Автобус #" << payment.busId << ": " << payment.amount << " руб. (за период " << payment.period << ")" << endl;
    };

    ReportService::saveCrewPayments(payments);
    cout << "Начисления сохранены в базу данных." << endl;

    cout << "\n4. Проверка начислений на сегодня:" << endl;
    double payment = ReportService::getCrewPaymentByDate(
        reportBus->getId(), "2023-06-15"
    );
    cout << "Начислено экипажу автобуса #" << reportBus->getId() << ": " << payment << " руб." << endl;

    delete reportBus;
    printSeparator();
}

int main() {
    setlocale(LC_ALL, "UTF8");
    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.initialize("tour_agency_demo.db")) {
        cerr << "Не удалось инициализировать базу данных!" << endl;
        return 1;
    };

    demoUsers();
    demoBuses();
    demoCrewMembers();
    demoRoutes();
    demoTrips();
    demoReports();

    db.close();
    return 0;
}