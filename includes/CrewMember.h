#ifndef CREWMEMBER_H
#define CREWMEMBER_H

#include <string>
#include <vector>

class Bus;

class CrewMember {
public:
    CrewMember(int id, const std::string& lastName, const std::string& employeeNumber,
        int experience, const std::string& category, const std::string& address,
        int birthYear, int busId);

    int getId() const { return id; }
    std::string getLastName() const { return lastName; }
    int getBusId() const { return busId; }
    std::string getEmployeeNumber() const { return employeeNumber; }
    int getExperience() const { return experience; }
    std::string getCategory() const { return category; }
    std::string getAddress() const { return address; }
    int getBirthYear() const { return birthYear; }

    static CrewMember* create(const std::string& lastName, const std::string& employeeNumber,
        int experience, const std::string& category, const std::string& address,
        int birthYear, int busId);
    bool update(const std::string& lastName, const std::string& employeeNumber,
        int experience, const std::string& category, const std::string& address,
        int birthYear, int busId);
    bool remove();
    static CrewMember* findById(int id);
    static std::vector<CrewMember*> findByBusId(int busId);
    static std::vector<CrewMember*> getAll();

private:
    int id;
    std::string lastName;
    std::string employeeNumber;
    int experience;
    std::string category;
    std::string address;
    int birthYear;
    int busId;
};

#endif