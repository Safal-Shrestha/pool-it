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

std::map<std::string, std::string> loadEnvFile(const std::string& filename) {
    std::map<std::string, std::string> envMap;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Unable to open .env file!" << std::endl;
        return envMap;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream is_line(line);
        std::string key, value;
        if (std::getline(is_line, key, '=') && std::getline(is_line, value)) {
            envMap[key] = value;
        }
    }

    return envMap;
}

namespace dbo = Wt::Dbo;

class User;
class Ride;
class RideUser;

namespace Wt {
    namespace Dbo {
        template<>
        struct dbo_traits<User> : public dbo_default_traits {
            static const char* versionField() {
                return nullptr;
            }

            static const char* surrogateIdField() {
                return nullptr;
            }
        };

        template<>
        struct dbo_traits<Ride> : public dbo_default_traits {
            static const char* versionField() {
                return nullptr;
            }

            static const char* surrogateIdField() {
                return nullptr;
            }
        };

        template<>
        struct dbo_traits<RideUser> : public dbo_default_traits {
            static const char* versionField() {
                return nullptr;
            }

            static const char* surrogateIdField() {
                return nullptr;
            }
        };
    }
}

class User{
public:
    int id;
    std::string uName;
    std::string password;
    std::string email;
    long long int phoneNo;
    std::string citizenship;
    std::chrono::system_clock::time_point createdAt;

    template<class Action>
    void persist(Action& a)
    {
        dbo::id(a, id, "userId");
        dbo::field(a, uName, "uName");
        dbo::field(a, password, "pw");
        dbo::field(a, email, "email");
        dbo::field(a, phoneNo, "phoneNo");
        dbo::field(a, citizenship, "citizenship");
        dbo::field(a, createdAt, "created_at");
    }
};

class Ride {
public:
    int id;
    std::string from;
    std::string to;
    int seats;
    std::chrono::system_clock::time_point createdAt;
    std::string status;

    template<class Action>
    void persist(Action& a)
    {
        dbo::id(a, id, "ride_id");
        dbo::field(a, from, "ride_from");
        dbo::field(a, to, "ride_to");
        dbo::field(a, seats, "seats");
        dbo::field(a, createdAt, "created_at");
        dbo::field(a, status, "status");
    }

};

class RideUser {
public:
    int id;
    int ride_id;
    int user_id;
    std::string user_mode;

    template<class Action>
    void persist(Action& a)
    {
        dbo::id(a, id, "id");
        dbo::field(a, ride_id, "ride_id");
        dbo::field(a, user_id, "user_id");
        dbo::field(a, user_mode, "user_mode");
    }
};

class DatabaseManager {
public:
    DatabaseManager(const std::string& db_pass) {
        auto mysqlBackend = std::make_unique<dbo::backend::MySQL>(
            "poolit_db",
            "wt_poolit",
            db_pass,
            "127.0.0.1",
            3306);

        session_.setConnection(std::move(mysqlBackend));
        session_.mapClass<User>("userdetails");
        session_.mapClass<Ride>("ride");
        session_.mapClass<RideUser>("ride_user");

        std::cout << "Database connected" << std::endl;
    }

    dbo::Session* getSession() {
        return &session_;
    }

private:
    dbo::Session session_;
};

class UserSession {
public:
    explicit UserSession(DatabaseManager& dbManager) : session_(*dbManager.getSession()), loggedInUser_() {}

    bool login(const std::string& email, const std::string& password) {
        dbo::Transaction transaction(session_);
        dbo::ptr<User> user = session_.find<User>().where("email = ?").bind(email);
        if (user) {
            std::cout << "User found: " << user->uName << std::endl;
            if (user->password == password) {
                std::cout << "Password matches" << std::endl;
                loggedInUser_ = user;
                return true;
            }
        }
        else {
            std::cout << "User not found" << std::endl;
        }
        return false;
    }

    void logout() {
        loggedInUser_ = nullptr;
    }

    bool isLoggedIn() const {
        return loggedInUser_.get() != nullptr;
    }

    dbo::ptr<User> getLoggedInUser() const {
        return loggedInUser_;
    }

private:
    dbo::Session& session_;
    dbo::ptr<User> loggedInUser_;
};


class MainApp : public Wt::WApplication {
public:
    MainApp(const Wt::WEnvironment& env, DatabaseManager& dbManager, UserSession& userSession);

private:
    DatabaseManager& dbManager_;
    UserSession& userSession_;
};

class Signup: public User, public Wt::WResource{
public:
    Signup(DatabaseManager& dbManager, UserSession& userSession)
        : session_(*dbManager.getSession()), userSession_(userSession) {
    }

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override
    {
        std::cout << "Signup called" << std::endl;
        if (request.method() == "POST") {
            std::stringstream buffer;
            buffer << request.in().rdbuf();
            std::string jsonBody = buffer.str();

            Wt::Json::Object jsonObject;
            Wt::Json::parse(jsonBody, jsonObject);

            std::string name = jsonObject.get("s_name").toString();
            std::string email = jsonObject.get("s_email").toString();
            long long int phone = static_cast<long long int>(jsonObject.get("s_phone").toNumber());
            std::string citizen = jsonObject.get("s_citizen").toString();
            std::string password = jsonObject.get("s_password").toString();


            try {
                Wt::Dbo::Transaction transaction(session_);

                Wt::Dbo::ptr<User> newUser = session_.add(std::make_unique<User>());
                newUser.modify()->uName = name;
                newUser.modify()->email = email;
                newUser.modify()->phoneNo = phone;
                newUser.modify()->password = password;
                newUser.modify()->citizenship = citizen;
                newUser.modify()->createdAt = std::chrono::system_clock::now();

                if (userSession_.login(email, password)) {
                    std::cout << "User Logged In Automatically" << std::endl;
                }
                else {
                    std::cout << "Login failed after signup" << std::endl;
                }
                transaction.commit();

                std::cout << "User Created" << std::endl;

                response.setStatus(200);
                response.out() << "{\"status\": \"success\", \"message\": \"User created successfully\"}";
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                response.setStatus(500);
                response.out() << "{\"status\": \"error\", \"message\": \"" << e.what() << "\"}";
            }
        }
    }

private:
    dbo::Session& session_;
    UserSession userSession_;
};

class Login : public Wt::WResource {
public:
    Login(DatabaseManager& dbManager, UserSession& userSession)
        : session_(*dbManager.getSession()), userSession_(userSession) {
    }

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override {
        if (request.method() == "POST") {
            std::stringstream buffer;
            buffer << request.in().rdbuf();
            std::string jsonBody = buffer.str();

            Wt::Json::Object jsonObject;
            Wt::Json::parse(jsonBody, jsonObject);

            std::string email = jsonObject.get("l_email").toString();
            std::string password = jsonObject.get("l_password").toString();

            try {
                if (userSession_.login(email, password)) {
                    dbo::ptr<User> loggedInUser = userSession_.getLoggedInUser();

                    response.setStatus(200);
                    response.out() << "{\"status\": \"success\", \"message\": \"Login successful\"}";
                }
                else {
                    std::cout << "Invalid credentials" << std::endl;

                    response.setStatus(401);
                    response.out() << "{\"status\": \"error\", \"message\": \"Invalid email or password\"}";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                response.setStatus(500);
                response.out() << "{\"status\": \"error\", \"message\": \"" << e.what() << "\"}";
            }
        }
        else
        {
            std::cout << "GET Used" << std::endl;
        }
    }

private:
    dbo::Session& session_;
    UserSession& userSession_;
};

class RideUpdate : public Wt::WResource {
public:
    RideUpdate(DatabaseManager& dbManager, UserSession& userSession)
        : session_(*dbManager.getSession()), userSession_(userSession) {
    }

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override {
        dbo::ptr<User> loggedInUser = userSession_.getLoggedInUser();

        std::cout << "User Logged In: " << loggedInUser->uName << std::endl;

        if (request.method() == "POST") {
            std::stringstream buffer;
            buffer << request.in().rdbuf();
            std::string jsonBody = buffer.str();

            std::cout << "Received login request with body: " << jsonBody << std::endl;

            Wt::Json::Object jsonObject;
            Wt::Json::parse(jsonBody, jsonObject);

            int user_id = loggedInUser->id;
            std::string r_from = jsonObject.get("from").toString();
            std::string r_to = jsonObject.get("to").toString();
            int r_seats = jsonObject.get("seats");
            std::string role = jsonObject.get("role").toString();
            std::string r_status = "matchmaking";

            try {
                if (loggedInUser) {

                    Wt::Dbo::Transaction transaction(session_);

                    Wt::Dbo::ptr<Ride> ride = session_.add(std::make_unique<Ride>());
                    ride.modify()->from = r_from;
                    ride.modify()->to = r_to;
                    ride.modify()->seats = r_seats;
                    ride.modify()->status = r_status;
                    ride.modify()->createdAt = std::chrono::system_clock::now();

                    transaction.commit();

                    response.setStatus(200);
                    response.out() << "{\"status\": \"success\", \"message\": \"User Logged In\"}";
                }
                else {
                    response.setStatus(401);
                    response.out() << "{\"status\": \"error\", \"message\": \"User Not Logged In\"}";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                response.setStatus(500);
                response.out() << "{\"status\": \"error\", \"message\": \"" << e.what() << "\"}";
            }
        }
    }

private:
    dbo::Session& session_;
    UserSession& userSession_;
};

class Logout : public Wt::WResource {
public:
    Logout(UserSession& userSession) : userSession_(userSession) {}

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override {
        if (request.method() == "POST") {
            try {
                userSession_.logout();
                std::cout << "Logged Out" << std::endl;
                response.setStatus(200);
                response.out() << "{\"status\": \"success\", \"message\": \"Logged out successfully\"}";
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

private:
    UserSession& userSession_;
};

#endif
