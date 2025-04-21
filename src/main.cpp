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
using std::string;
using std::vector;

#undef max

void clearInputBuffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printHeader(const string& title) {
    cout << "\n=== " << title << " ===\n";
}

void manageBuses() {
    while (true) {
        printHeader("УПРАВЛЕНИЕ АВТОБУСАМИ");
        cout << "1. Просмотреть все автобусы\n"
            << "2. Добавить автобус\n"
            << "3. Редактировать автобус\n"
            << "4. Удалить автобус\n"
            << "0. Назад\n"
            << "Выберите действие: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: {
            printHeader("СПИСОК АВТОБУСОВ");
            vector<Bus*> buses = Bus::getAll();
            for (const auto& bus : buses) {
                cout << "ID: " << bus->getId()
                    << " | Номер: " << bus->getNumber()
                    << " | Модель: " << bus->getModel()
                    << " | Пробег: " << bus->getTotalMileage() << " км\n";
                delete bus;
            }
            break;
        }
        case 2: {
            printHeader("ДОБАВЛЕНИЕ АВТОБУСА");
            string number, model;
            double mileage;

            cout << "Введите номер автобуса: ";
            getline(cin, number);
            cout << "Введите модель автобуса: ";
            getline(cin, model);
            cout << "Введите пробег автобуса: ";
            cin >> mileage;
            clearInputBuffer();

            Bus* newBus = Bus::create(number, model, mileage);
            if (newBus) {
                cout << "Автобус успешно добавлен!\n";
                delete newBus;
            }
            else {
                cout << "Ошибка при добавлении автобуса!\n";
            }
            break;
        }
        case 3: {
            printHeader("РЕДАКТИРОВАНИЕ АВТОБУСА");
            int id;
            cout << "Введите ID автобуса для редактирования: ";
            cin >> id;
            clearInputBuffer();

            Bus* bus = Bus::findById(id);
            if (bus) {
                string number, model;
                double totalMileage;

                cout << "Текущий номер: " << bus->getNumber() << ".\nВведите новый номер: ";
                getline(cin, number);
                cout << "Текущая модель: " << bus->getModel() << ".\nВведите новую модель: ";
                getline(cin, model);
                cout << "Текущий пробег: " << bus->getTotalMileage() << ".\nВведите новый пробег: ";
                cin >> totalMileage;
                clearInputBuffer();

                if (bus->update(number, model, totalMileage)) {
                    cout << "Автобус успешно отредактирован!\n";
                }
                else {
                    cout << "Ошибка при редактировании автобуса!\n";
                }
                delete bus;
            }
            else {
                cout << "Автобус с таким ID не найден!\n";
            }
            break;
        }
        case 4: {
            printHeader("УДАЛЕНИЕ АВТОБУСА");
            int id;
            cout << "Введите ID автобуса для удаления: ";
            cin >> id;
            clearInputBuffer();

            Bus* bus = Bus::findById(id);
            if (bus) {
                if (bus->remove()) {
                    cout << "Автобус успешно удалён!\n";
                }
                else {
                    cout << "Ошибка при удалении автобуса!\n";
                }
            }
            else {
                cout << "Автобус с таким ID не найден!\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        }
    }
}

void manageCrewMembers() {
    while (true) {
        printHeader("УПРАВЛЕНИЕ ЭКИПАЖАМИ");
        cout << "1. Просмотреть всех членов экипажа\n"
            << "2. Добавить члена экипажа\n"
            << "3. Редактировать члена экипажа\n"
            << "4. Удалить члена экипажа\n"
            << "0. Назад\n"
            << "Выберите действие: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: {
            printHeader("СПИСОК ЧЛЕНОВ ЭКИПАЖА");
            vector<CrewMember*> crew = CrewMember::getAll();
            for (const auto& member : crew) {
                cout << "ID: " << member->getId()
                    << " | Фамилия: " << member->getLastName()
                    << " | Табельный №: " << member->getEmployeeNumber()
                    << " | Автобус ID: " << member->getBusId() << "\n";
                delete member;
            }
            break;
        }
        case 2: {
            printHeader("ДОБАВЛЕНИЕ ЧЛЕНА ЭКИПАЖА");
            string lastName, employeeNumber, category, address;
            int experience, birthYear, busId;

            cout << "Введите фамилию: ";
            getline(cin, lastName);
            cout << "Введите табельный номер: ";
            getline(cin, employeeNumber);
            cout << "Введите стаж: ";
            cin >> experience;
            clearInputBuffer();
            cout << "Введите категорию: ";
            getline(cin, category);
            cout << "Введите адрес: ";
            getline(cin, address);
            cout << "Введите год рождения: ";
            cin >> birthYear;
            cout << "Введите ID автобуса: ";
            cin >> busId;
            clearInputBuffer();

            CrewMember* newMember = CrewMember::create(lastName, employeeNumber, experience,
                category, address, birthYear, busId);
            if (newMember) {
                cout << "Член экипажа успешно добавлен!\n";
                delete newMember;
            }
            else {
                cout << "Ошибка при добавлении члена экипажа!\n";
            }
            break;
        }
        case 3: {
            printHeader("РЕДАКТИРОВАНИЕ ЧЛЕНА ЭКИПАЖА");
            int id;
            cout << "Введите ID члена экипажа для редактирования: ";
            cin >> id;
            clearInputBuffer();

            CrewMember* member = CrewMember::findById(id);
            if (member) {
                string lastName, employeeNumber, category, address;
                int experience, birthYear, busId;

                cout << "Текущая фамилия: " << member->getLastName() << ".\nВведите новую фамилию: ";
                getline(cin, lastName);
                cout << "Текущий табельный номер: " << member->getEmployeeNumber() << ".\nВведите новый табельный номер: ";
                getline(cin, employeeNumber);
                cout << "Текущий стаж: " << member->getExperience() << ".\nВведите новый стаж: ";
                cin >> experience;
                cout << "Текущая категория: " << member->getCategory() << ".\nВведите новую категорию: ";
                getline(cin, category);
                cout << "Текущий адрес: " << member->getAddress() << ".\nВведите новый адрес: ";
                getline(cin, address);
                cout << "Текущий год рождения: " << member->getBirthYear() << ".\nВведите новый год рождения: ";
                cin >> birthYear;
                cout << "Текущий ID автобуса: " << member->getBusId() << ".\nВведите новый ID автобуса: ";
                cin >> busId;
                clearInputBuffer();

                if (member->update(lastName, employeeNumber, experience, category, address, birthYear, busId)) {
                    cout << "Член экипажа успешно отредактирован!\n";
                }
                else {
                    cout << "Ошибка при редактировании члена экипажа!\n";
                }
                delete member;
            }
            else {
                cout << "Член экипажа с таким ID не найден!\n";
            }
            break;
        }
        case 4: {
            printHeader("УДАЛЕНИЕ ЧЛЕНА ЭКИПАЖА");
            int id;
            cout << "Введите ID члена экипажа для удаления: ";
            cin >> id;
            clearInputBuffer();

            CrewMember* member = CrewMember::findById(id);
            if (member) {
                if (member->remove()) {
                    cout << "Член экипажа успешно удалён!\n";
                }
                else {
                    cout << "Ошибка при удалении члена экипажа!\n";
                }
            }
            else {
                cout << "Член экипажа с таким ID не найден!\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        }
    }
}

void manageRoutes() {
    while (true) {
        printHeader("УПРАВЛЕНИЕ МАРШРУТАМИ");
        cout << "1. Просмотреть все маршруты\n"
            << "2. Добавить маршрут\n"
            << "3. Редактировать маршрут\n"
            << "4. Удалить маршрут\n"
            << "0. Назад\n"
            << "Выберите действие: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: {
            printHeader("СПИСОК МАРШРУТОВ");
            vector<Route*> routes = Route::getAll();
            for (const auto& route : routes) {
                cout << "ID: " << route->getId()
                    << " | Маршрут: " << route->getName()
                    << " | " << route->getStartPoint() << " -> " << route->getEndPoint()
                    << " | " << route->getDistance() << " км\n";
                delete route;
            }
            break;
        }
        case 2: {
            printHeader("ДОБАВЛЕНИЕ МАРШРУТА");
            string name, startPoint, endPoint;
            double distance;

            cout << "Введите название маршрута: ";
            getline(cin, name);
            cout << "Введите начальную точку: ";
            getline(cin, startPoint);
            cout << "Введите конечную точку: ";
            getline(cin, endPoint);
            cout << "Введите расстояние (км): ";
            cin >> distance;
            clearInputBuffer();

            Route* newRoute = Route::create(name, startPoint, endPoint, distance);
            if (newRoute) {
                cout << "Маршрут успешно добавлен!\n";
                delete newRoute;
            }
            else {
                cout << "Ошибка при добавлении маршрута!\n";
            }
            break;
        }
        case 3: {
            printHeader("РЕДАКТИРОВАНИЕ МАРШРУТА");
            int id;
            cout << "Введите ID маршрута для редактирования: ";
            cin >> id;
            clearInputBuffer();

            Route* route = Route::findById(id);
            if (route) {
                string name, startPoint, endPoint;
                double distance;
                
                cout << "Текущее название маршрута: " << route->getName() << ".\nВведите новое название маршрута: ";
                getline(cin, name);
                cout << "Текущая начальная точка: " << route->getStartPoint() << ".\nВведите новую начальную точку: ";
                getline(cin, startPoint);
                cout << "Текущая конечная точка: " << route->getEndPoint() << ".\nВведите новую конечную точку: ";
                getline(cin, endPoint);
                cout << "Текущее расстояние (км): " << route->getDistance() << ".\nВведите новое расстояние (км): ";
                cin >> distance;
                clearInputBuffer();

                if (route->update(name, startPoint, endPoint, distance)) {
                    cout << "Маршрут успешно отредактирован!\n";
                }
                else {
                    cout << "Ошибка при редактировании маршрута!\n";
                }
                delete route;
            }
            else {
                cout << "Маршрут с таким ID не найден!\n";
            }
            break;
        }
        case 4: {
            printHeader("УДАЛЕНИЕ МАРШРУТА");
            int id;
            cout << "Введите ID маршрута для удаления: ";
            cin >> id;
            clearInputBuffer();

            Route* route = Route::findById(id);
            if (route) {
                if (route->remove()) {
                    cout << "Маршрут успешно удалеён!\n";
                }
                else {
                    cout << "Ошибка при удалении маршрута!\n";
                }
            }
            else {
                cout << "Маршрут с таким ID не найдена!\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        }
    }
}

void manageTrips() {
    while (true) {
        printHeader("УПРАВЛЕНИЕ ПОЕЗДКАМИ");
        cout << "1. Просмотреть все поездки\n"
            << "2. Добавить поездку\n"
            << "3. Редактировать поездку\n"
            << "4. Удалить поездку\n"
            << "0. Назад\n"
            << "Выберите действие: ";

        int choice;
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
        case 1: {
            printHeader("СПИСОК ПОЕЗДОК");
            vector<Trip*> trips = Trip::getAll();
            for (const auto& trip : trips) {
                cout << "ID: " << trip->getId()
                    << " | Автобус ID: " << trip->getBusId()
                    << " | " << trip->getDepartureDate() << " - " << trip->getArrivalDate()
                    << " | Маршрут: " << trip->getRouteName()
                    << " | Пассажиров: " << trip->getPassengersCount()
                    << " | Цена: " << trip->getTicketPrice() << " руб.\n";
                delete trip;
            }
            break;
        }
        case 2: {
            printHeader("ДОБАВЛЕНИЕ ПОЕЗДКИ");
            int busId, passengersCount;
            double ticketPrice;
            string departureDate, arrivalDate, routeName;

            cout << "Введите ID автобуса: ";
            cin >> busId;
            clearInputBuffer();
            cout << "Введите дату отправления (ГГГГ-ММ-ДД): ";
            getline(cin, departureDate);
            cout << "Введите дату прибытия (ГГГГ-ММ-ДД): ";
            getline(cin, arrivalDate);
            cout << "Введите название маршрута: ";
            getline(cin, routeName);
            cout << "Введите количество пассажиров: ";
            cin >> passengersCount;
            cout << "Введите цену билета: ";
            cin >> ticketPrice;
            clearInputBuffer();

            Trip* newTrip = Trip::create(busId, departureDate, arrivalDate,
                routeName, passengersCount, ticketPrice);
            if (newTrip) {
                cout << "Поездка успешно добавлена!\n";
                delete newTrip;
            }
            else {
                cout << "Ошибка при добавлении поездки!\n";
            }
            break;
        }
        case 3: {
            printHeader("РЕДАКТИРОВАНИЕ ПОЕЗДКИ");
            int id;
            cout << "Введите ID поездки для редактирования: ";
            cin >> id;
            clearInputBuffer();

            Trip* trip = Trip::findById(id);
            if (trip) {
                int busId, passengersCount;
                string departureDate, arrivalDate, routeName;
                double ticketPrice;

                cout << "Текущее ID автобуса: " << trip->getBusId() << ".\nВведите новый ID автобуса: ";
                cin >> busId;
                cout << "Текущая дата отправления (ГГГГ-ММ-ДД): " << trip->getDepartureDate() << ".\nВведите новую дату отправления (ГГГГ-ММ-ДД): ";
                getline(cin, departureDate);
                cout << "Текущая дата прибытия (ГГГГ-ММ-ДД): " << trip->getArrivalDate() << ".\nВведите новую дату прибытия (ГГГГ-ММ-ДД): ";
                getline(cin, arrivalDate);
                cout << "Текущее название маршрута: " << trip->getRouteName() << ".\nВведите новое название маршрута: ";
                getline(cin, routeName);
                cout << "Текущее количество пассажиров: " << trip->getPassengersCount() << ".\nВведите новое количество пассажиров: ";
                cin >> passengersCount;
                cout << "Текущая цена билета: " << trip->getTicketPrice() << ".\nВведите новую цену билета: ";
                cin >> ticketPrice;
                clearInputBuffer();

                if (trip->update(busId, departureDate, arrivalDate, routeName, passengersCount, ticketPrice)) {
                    cout << "Поездка успешно отредактирована!\n";
                }
                else {
                    cout << "Ошибка при редактировании поездки!\n";
                }
                delete trip;
            }
            else {
                cout << "Поездка с таким ID не найдена!\n";
            }
            break;
        }
        case 4: {
            printHeader("УДАЛЕНИЕ ПОЕЗДКИ");
            int id;
            cout << "Введите ID поездки для удаления: ";
            cin >> id;
            clearInputBuffer();

            Trip* trip = Trip::findById(id);
            if (trip) {
                if (trip->remove()) {
                    cout << "Поездка успешно удалена!\n";
                }
                else {
                    cout << "Ошибка при удалении поездки!\n";
                }
            }
            else {
                cout << "Поездка с таким ID не найдена!\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        }
    }
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
        case 1:
            manageBuses();
            break;
        case 2:
            manageCrewMembers();
            break;
        case 3:
            manageRoutes();
            break;
        case 4:
            manageTrips();
            break;
        case 5: {
            while (true) {
                printHeader("ОТЧЕТЫ");
                cout << "1. Рейсы автобуса за период\n"
                    << "2. Сводка по автобусу\n"
                    << "3. Начисления экипажам\n"
                    << "0. Назад\n"
                    << "Выберите отчет: ";

                int reportChoice;
                cin >> reportChoice;
                clearInputBuffer();

                switch (reportChoice) {
                case 1: {
                    printHeader("РЕЙСЫ АВТОБУСА ЗА ПЕРИОД");
                    int busId;
                    string startDate, endDate;

                    cout << "Введите ID автобуса: ";
                    cin >> busId;
                    clearInputBuffer();
                    cout << "Введите начальную дату (ГГГГ-ММ-ДД): ";
                    getline(cin, startDate);
                    cout << "Введите конечную дату (ГГГГ-ММ-ДД): ";
                    getline(cin, endDate);

                    vector<TripReport> reports = ReportService::getBusTrips(busId, startDate, endDate);
                    if (reports.empty()) {
                        cout << "Рейсов за указанный период не найдено.\n";
                    }
                    else {
                        cout << "\nРейсы автобуса " << busId << " за период с " << startDate << " по " << endDate << ":\n";
                        for (const auto& report : reports) {
                            cout << "Дата отправления: " << report.departureDate
                                << " | Дата прибытия: " << report.arrivalDate
                                << " | Маршрут: " << report.routeName
                                << " | Пассажиров: " << report.passengersCount
                                << " | Цена билета: " << report.ticketPrice << " руб.\n";
                        }
                    }
                    break;
                }
                case 2: {
                    printHeader("СВОДКА ПО АВТОБУСУ");
                    int busId;
                    string startDate, endDate;

                    cout << "Введите ID автобуса: ";
                    cin >> busId;
                    clearInputBuffer();
                    cout << "Введите начальную дату (ГГГГ-ММ-ДД): ";
                    getline(cin, startDate);
                    cout << "Введите конечную дату (ГГГГ-ММ-ДД): ";
                    getline(cin, endDate);

                    double revenue = ReportService::calculateBusRevenue(busId, startDate, endDate);
                    cout << "\nСводка по автобусу " << busId << " за период с " << startDate << " по " << endDate << ":\n";
                    vector<TripReport> trips = ReportService::getBusTrips(busId, startDate, endDate);
                    cout << "Количество рейсов: " << trips.size() << "\n";
                    cout << "Общая выручка: " << revenue << " руб.\n";

                    break;
                }
                case 3: {
                    printHeader("НАЧИСЛЕНИЯ ЭКИПАЖАМ");
                    double percentage;
                    string startDate, endDate;

                    cout << "Введите процент от выручки для начисления: ";
                    cin >> percentage;
                    clearInputBuffer();
                    cout << "Введите начальную дату периода (ГГГГ-ММ-ДД): ";
                    getline(cin, startDate);
                    cout << "Введите конечную дату периода (ГГГГ-ММ-ДД): ";
                    getline(cin, endDate);

                    vector<CrewPayment> payments = ReportService::calculateCrewPayments(percentage, startDate, endDate);
                    if (payments.empty()) {
                        cout << "Нет данных для начислений за указанный период.\n";
                    }
                    else {
                        cout << "\nНачисления экипажам за период с " << startDate << " по " << endDate << ":\n";
                        for (const auto& payment : payments) {
                            cout << "Автобус ID: " << payment.busId
                                << " | Период: " << payment.period
                                << " | Сумма: " << payment.amount << " руб.\n";
                        }

                        cout << "\nСохранить начисления в базу данных? (y/n): ";
                        char choice;
                        cin >> choice;
                        clearInputBuffer();

                        if (choice == 'y' || choice == 'Y') {
                            ReportService::saveCrewPayments(payments);
                            cout << "Начисления успешно сохранены!\n";
                        }
                    }
                    break;
                }
                case 0:
                    break;
                default:
                    cout << "Неверный выбор!\n";
                }

                if (reportChoice == 0) break;
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        }
    }
}

void showCrewMenu(sqlite3* db, int crewMemberId) {
    int busId = CrewMember::findById(crewMemberId)->getBusId();
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
            Bus* bus = Bus::findById(busId);
            if (bus) {
                printHeader("ИНФОРМАЦИЯ ОБ АВТОБУСЕ");
                cout << "Номер: " << bus->getNumber() << "\n"
                    << "Модель: " << bus->getModel() << "\n"
                    << "Пробег: " << bus->getTotalMileage() << " км\n";
                delete bus;
            }
            else {
                cout << "Автобус не найден!\n";
            };
            break;
        }
        case 2: {
            printHeader("ИНФОРМАЦИЯ ОБ ЭКИПАЖЕ");
            vector<CrewMember*> crew = CrewMember::findByBusId(busId);
            for (const auto& member : crew) {
                cout << "Фамилия: " << member->getLastName() << "\n"
                    << "Категория: " << member->getCategory() << "\n"
                    << "Стаж: " << member->getExperience() << " лет\n"
                    << "Год рождения: " << member->getBirthYear() << "\n";
                delete member;
            };
            break;
        }
        case 3: {
            printHeader("НАЧИСЛЕНИЯ");
            cout << "Введите дату (ГГГГ-ММ-ДД): ";
            string date;
            getline(cin, date);

            double payment = ReportService::getCrewPaymentByDate(busId, date);
            if (payment > 0) {
                cout << "Начислено: " << payment << " руб.\n";
            }
            else {
                cout << "Начислений на эту дату не найдено.\n";
            };
            break;
        }
        case 0:
            return;
        default:
            cout << "Неверный выбор!\n";
        };
    };
}
int main() {
    setlocale(LC_ALL, "UTF8");

    DatabaseManager& dbManager = DatabaseManager::getInstance();
    if (!dbManager.initialize("tour_agency.db")) {
        cerr << "Не удалось инициализировать базу данных!\n";
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
            cout << "\nДобро пожаловать, " << user->getUsername() << "!\n";

            if (user->getRole() == User::Role::Admin) {
                showAdminMenu(db);
            }
            else {
                showCrewMenu(db, user->getCrewMemberId());
            }

            delete user;
        }
        else {
            cout << "Ошибка аутентификации! Неверный логин или пароль.\n";
        }
    }

    dbManager.close();
    return 0;
}

