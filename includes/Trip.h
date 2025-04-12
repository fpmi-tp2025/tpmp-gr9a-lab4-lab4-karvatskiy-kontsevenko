#ifndef TRIP_H
#define TRIP_H

#include <string>
#include <vector>

class Bus;
class Route;

class Trip {
public:
    Trip(int id, int busId, const std::string& departureDate, const std::string& arrivalDate,
        const std::string& routeName, int passengersCount, double ticketPrice);

    int getId() const { return id; }
    int getBusId() const { return busId; }
    std::string getDepartureDate() const { return departureDate; }
    std::string getArrivalDate() const { return arrivalDate; }
    std::string getRouteName() const { return routeName; }
    int getPassengersCount() const { return passengersCount; }
    double getTicketPrice() const { return ticketPrice; }
    static std::vector<Trip*> getAll();

    static Trip* create(int busId, const std::string& departureDate, const std::string& arrivalDate,
        const std::string& routeName, int passengersCount, double ticketPrice);
    static std::vector<Trip*> findByBusId(int busId);
    static std::vector<Trip*> findByDateRange(const std::string& startDate, const std::string& endDate);

private:
    int id;
    int busId;
    std::string departureDate;
    std::string arrivalDate;
    std::string routeName;
    int passengersCount;
    double ticketPrice;
};

#endif