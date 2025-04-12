#ifndef REPORTSERVICE_H
#define REPORTSERVICE_H

#include <string>
#include <vector>

class Bus;
class CrewMember;

struct TripReport {
    std::string departureDate;
    std::string arrivalDate;
    std::string routeName;
    int passengersCount;
    double ticketPrice;
};

struct CrewPayment {
    int busId;
    double amount;
    std::string period;
};

class ReportService {
public:
    static std::vector<TripReport> getBusTrips(int busId, const std::string& startDate, const std::string& endDate);
    static double calculateBusRevenue(int busId, const std::string& startDate, const std::string& endDate);
    static std::vector<CrewPayment> calculateCrewPayments(double percentage, const std::string& startDate, const std::string& endDate);
    static void saveCrewPayments(const std::vector<CrewPayment>& payments);
    static double getCrewPaymentByDate(int busId, const std::string& date);
};

#endif