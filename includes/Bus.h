#ifndef BUS_H
#define BUS_H

#include <string>
#include <vector>

class CrewMember;

class Bus {
public:
    Bus(int id, const std::string& number, const std::string& model, double totalMileage);

    int getId() const { return id; }
    std::string getNumber() const { return number; }
    std::string getModel() const { return model; }
    double getTotalMileage() const { return totalMileage; }

    void setTotalMileage(double mileage) { totalMileage = mileage; }

    static Bus* create(const std::string& number, const std::string& model, double totalMileage);
    bool update(const std::string& number, const std::string& model, double totalMileage);
    bool remove();
    static Bus* findById(int id);
    static std::vector<Bus*> getAll();

    std::vector<CrewMember*> getCrewMembers() const;

private:
    int id;
    std::string number;
    std::string model;
    double totalMileage;
};

#endif