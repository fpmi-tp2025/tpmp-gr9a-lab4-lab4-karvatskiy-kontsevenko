#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>

class Trip;

class Route {
public:
    Route(int id, const std::string& name, const std::string& startPoint,
        const std::string& endPoint, double distance);

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getStartPoint() const { return startPoint; }
    std::string getEndPoint() const { return endPoint; }
    double getDistance() const { return distance; }

    static Route* create(const std::string& name, const std::string& startPoint,
        const std::string& endPoint, double distance);
    static Route* findById(int id);
    static std::vector<Route*> getAll();

    std::vector<Trip*> getTrips() const;

private:
    int id;
    std::string name;
    std::string startPoint;
    std::string endPoint;
    double distance;
};

#endif