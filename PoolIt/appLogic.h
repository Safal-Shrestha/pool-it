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
#include <Wt/Json/Object.h>
#include <Wt/Json/Parser.h>
#include <Wt/Json/Serializer.h>
#include <map>

namespace dbo = Wt::Dbo;

class User{
public:
    int id;
    std::string uName;
    std::string password;
    std::string email;
    long int phoneNo;
    std::string citizenship;
    std::chrono::system_clock::time_point createdAt;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, id, "id");
        dbo::field(a, uName, "uName");
        dbo::field(a, password, "password");
        dbo::field(a, email, "email");
        dbo::field(a, phoneNo, "phoneNo");
        dbo::field(a, citizenship, "citizenship");
        dbo::field(a, createdAt, "createdAt");
    }
};

class MainApp : public Wt::WApplication {
public:
    MainApp(const Wt::WEnvironment& env);
    
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

class UserSession {
public:
    explicit UserSession(Wt::Dbo::Session& session) : session_(session) {}

    bool login(const std::string& username, const std::string& password) {
        Wt::Dbo::Transaction transaction(session_);
        Wt::Dbo::ptr<User> user = session_.find<User>().where("uName = ?").bind(username);

        if (user && user->password == password) {
            sessionId_ = std::to_string(user->id);
            return true;
        }
        return false;
    }

    void logout() {
        sessionId_.clear();
    }

    bool isAuthenticated() {
        return !sessionId_.empty();
    }

private:
    Wt::Dbo::Session& session_;
    std::string sessionId_;
};


class Signup: public User, public Wt::WResource{
public:
    Wt::Dbo::Session& session_;

    explicit Signup(Wt::Dbo::Session& session) : session_(session) {}

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override
    {
        if (request.method() == "POST") {
            std::stringstream buffer;
            buffer << request.in().rdbuf();
            std::string jsonBody = buffer.str();

            Wt::Json::Object jsonObject;
            Wt::Json::parse(jsonBody, jsonObject);

            std::string name = jsonObject.get("s_name").toString();
            std::string email = jsonObject.get("s_email").toString();
            long int phone = static_cast<long int>(jsonObject.get("s_phone").toNumber());
            std::string citizen = jsonObject.get("s_citizen").toString();
            std::string password = jsonObject.get("s_password").toString();

            Wt::Dbo::Transaction transaction(session_);
            
            Wt::Dbo::ptr<User> existingUser = session_.find<User>().where("email = ?").bind(email);
            if (existingUser) {
                transaction.commit();
                response.setStatus(400);
                response.out() << "{\"status\": \"error\",\"message\": \"Email already registered\"}";
                return;
            }

            Wt::Dbo::ptr<User> newUser = session_.add(std::make_unique<User>());
            newUser.modify()->uName = name;
            newUser.modify()->email = email;
            newUser.modify()->phoneNo = phone;
            newUser.modify()->password = password;
            newUser.modify()->citizenship = citizen;
            newUser.modify()->createdAt = std::chrono::system_clock::now();


            transaction.commit();
        }
    }
};


class RideUpdate : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override{
        if (request.method() == "POST") {
            std::string jsonBody;
            request.in() >> jsonBody;

            Wt::Json::Object jsonObject;
            Wt::Json::parse(jsonBody, jsonObject);

            std::string from = jsonObject.get("from");
            std::string to = jsonObject.get("to");
            std::string time = jsonObject.get("time");
            int seats = jsonObject.get("seats");

            std::cout << "Received JSON Data: " << from << " to " << to << " at " << time << " (" << seats << " seats)" << std::endl;
        }
    }
};

#endif
