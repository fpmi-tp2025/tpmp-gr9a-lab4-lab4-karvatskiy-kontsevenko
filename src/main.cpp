#include "../includes/DatabaseManager.h"
#include "../includes/User.h"
#include "../includes/Bus.h"
#include "../includes/CrewMember.h"
#include "../includes/Route.h"
#include "../includes/Trip.h"
#include "../includes/ReportService.h"
#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

#undef max

void clearInputBuffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printHeader(const string& title) {
    cout << "\n=== " << title << " ===" << endl;
}

void showAdminMenu(sqlite3* db) {
    while (true) {
        printHeader("АДМИНИСТРАТОР");
        cout << "1. Управление автобусами\n"
            << "2. Управление экипажами\n"
            << "3. Управление маршрутами\n"
            << "4. Управление поездками\n"
            << "5. Просмотр отчетов\n"
            << "0. Выход\n"
            << "Выберите действие: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: {
            printHeader("УПРАВЛЕНИЕ АВТОБУСАМИ");
            vector<Bus*> buses = Bus::getAll();
            for (const auto& bus : buses) {
                cout << "ID: " << bus->getId()
                    << " | Номер: " << bus->getNumber()
                    << " | Модель: " << bus->getModel()
                    << " | Пробег: " << bus->getTotalMileage() << " км" << endl;
                delete bus;
            }
            break;
        }
        case 2: {
            printHeader("УПРАВЛЕНИЕ ЭКИПАЖАМИ");
            vector<CrewMember*> crew = CrewMember::getAll();
            for (const auto& member : crew) {
                cout << "ID: " << member->getId()
                    << " | Фамилия: " << member->getLastName()
                    << " | Табельный №: " << member->getEmployeeNumber()
                    << " | Автобус ID: " << member->getBusId() << endl;
                delete member;
            }
            break;
        }
        case 3: {
            printHeader("УПРАВЛЕНИЕ МАРШРУТАМИ");
            vector<Route*> routes = Route::getAll();
            for (const auto& route : routes) {
                cout << "ID: " << route->getId()
                    << " | Маршрут: " << route->getName()
                    << " | " << route->getStartPoint() << " -> " << route->getEndPoint()
                    << " | " << route->getDistance() << " км" << endl;
                delete route;
            }
            break;
        }
        case 4: {
            printHeader("УПРАВЛЕНИЕ ПОЕЗДКАМИ");
            vector<Trip*> trips = Trip::getAll();
            for (const auto& trip : trips) {
                cout << "ID: " << trip->getId()
                    << " | Автобус ID: " << trip->getBusId()
                    << " | " << trip->getDepartureDate() << " - " << trip->getArrivalDate()
                    << " | Маршрут: " << trip->getRouteName()
                    << " | Пассажиров: " << trip->getPassengersCount()
                    << " | Цена: " << trip->getTicketPrice() << " руб." << endl;
                delete trip;
            }
            break;
        }
        case 5: {
            printHeader("ОТЧЕТЫ");
            cout << "1. Рейсы автобуса за период\n"
                << "2. Сводка по автобусу\n"
                << "3. Начисления экипажам\n"
                << "Выберите отчет: ";

            int reportChoice;
            cin >> reportChoice;
            clearInputBuffer();

            // Реализация отчетов...
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!" << endl;
        };
    };
}

void showCrewMenu(sqlite3* db, int crewBusId) {
    while (true) {
        printHeader("МЕНЮ ЭКИПАЖА");
        cout << "1. Просмотреть информацию о своем автобусе\n"
            << "2. Просмотреть информацию о своем экипаже\n"
            << "3. Просмотреть свои начисления\n"
            << "0. Выход\n"
            << "Выберите действие: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: {
            Bus* bus = Bus::findById(crewBusId);
            if (bus) {
                printHeader("ИНФОРМАЦИЯ ОБ АВТОБУСЕ");
                cout << "Номер: " << bus->getNumber() << "\n"
                    << "Модель: " << bus->getModel() << "\n"
                    << "Пробег: " << bus->getTotalMileage() << " км" << endl;
                delete bus;
            }
            else {
                cout << "Автобус не найден!" << endl;
            };
            break;
        }
        case 2: {
            printHeader("ИНФОРМАЦИЯ ОБ ЭКИПАЖЕ");
            vector<CrewMember*> crew = CrewMember::findByBusId(crewBusId);
            for (const auto& member : crew) {
                cout << "Фамилия: " << member->getLastName() << "\n"
                    << "Категория: " << member->getCategory() << "\n"
                    << "Стаж: " << member->getExperience() << " лет\n"
                    << "Год рождения: " << member->getBirthYear() << endl;
                delete member;
            };
            break;
        }
        case 3: {
            printHeader("НАЧИСЛЕНИЯ");
            cout << "Введите дату (ГГГГ-ММ-ДД): ";
            string date;
            getline(cin, date);

            double payment = ReportService::getCrewPaymentByDate(crewBusId, date);
            if (payment > 0) {
                cout << "Начислено: " << payment << " руб." << endl;
            }
            else {
                cout << "Начислений на эту дату не найдено." << endl;
            };
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!" << endl;
        };
    };
}
int main() {
    system("chcp 1251");
    setlocale(LC_ALL, "UTF8");

    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (!dbManager.initialize("tour_agency.db")) {
        cerr << "Не удалось инициализировать базу данных!" << endl;
        return 1;
    }

    sqlite3* db = dbManager.getDatabase();

    while (true) {
        printHeader("АУТЕНТИФИКАЦИЯ");
        cout << "Введите логин (или 'exit' для выхода): ";
        string username;
        getline(cin, username);

        if (username == "exit") {
            break;
        }

        cout << "Введите пароль: ";
        string password;
        getline(cin, password);

        User* user = User::authenticate(username, password);
        if (user) {
            cout << "\nДобро пожаловать, " << user->getUsername() << "!" << endl;

            if (user->getRole() == User::Role::Admin) {
                showAdminMenu(db);
            }
            else {
                showCrewMenu(db, user->getCrewBusId());
            }

            delete user;
        }
        else {
            cout << "Ошибка аутентификации! Неверный логин или пароль." << endl;
        }
    }

    dbManager.close();
    return 0;
}

