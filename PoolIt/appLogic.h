#ifndef MAINAPP_H
#define MAINAPP_H

#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/WEnvironment.h>
#include <Wt/WResource.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <Wt/WDate.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/MySQL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

namespace dbo = Wt::Dbo;

enum class userMode
{
    admin = 0,
    rider = 1,
    passenger = 2
};

enum class userGender
{
    male = 0,
    female = 1
};

class user{
public:
    int id;
    std::string uName;
    std::string pw;
    std::string email;
    long int phoneNo;
    userMode uMode;
    userGender gender;
    Wt::WDate dob;
    Wt::WDateTime createdAt;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, id, id);
        dbo::field(a, uName, uName);
        dbo::field(a, pw, pw);
        dbo::field(a, email, email);
        dbo::field(a, phoneNo, phoneNo);
        dbo::field(a, uMode, uMode);
        dbo::field(a, gender, gender);
        dbo::field(a, dob, dob);
        dbo::field(a, createdAt, createdAt);
    }
};

class MainApp : public Wt::WApplication {
public:
    MainApp(const Wt::WEnvironment& env);
};

// Class to handle button click request
class ButtonClickHandler : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;
};

std::map<std::string, std::string> loadEnvFile(const std::string& filename) {
    std::map<std::string, std::string> envMap;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Unable to open .env file!" << std::endl;
        return envMap;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Ignore empty lines and comments

        std::istringstream is_line(line);
        std::string key, value;
        if (std::getline(is_line, key, '=') && std::getline(is_line, value)) {
            envMap[key] = value;
        }
    }

    return envMap;
}


#endif
