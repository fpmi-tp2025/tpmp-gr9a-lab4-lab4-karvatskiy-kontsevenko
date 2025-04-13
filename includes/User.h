#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    enum class Role { Admin, Crew };

    User(int id, const std::string& username, const std::string& password, Role role, int crewMemberId);

    int getId() const { return id; }
    std::string getUsername() const { return username; }
    Role getRole() const { return role; }
    int getCrewMemberId() const { return crewMemberId; }

    static User* create(const std::string& username, const std::string& password, const std::string& role, int crewMemberId);
    bool authenticate(const std::string& password) const;
    static User* authenticate(const std::string& username, const std::string& password);

private:
    int id;
    std::string username;
    std::string password;
    Role role;
    int crewMemberId;
};

#endif