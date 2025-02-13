#include "appLogic.h"

class StaticFileResource : public Wt::WResource {
public:
    explicit StaticFileResource(const std::string& filePath) : filePath_(filePath) {}

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override {
        std::ifstream file(filePath_, std::ios::in | std::ios::binary);
        if (!file) {
            response.setStatus(404);
            response.out() << "File Not Found";
            return;
        }

        response.setMimeType("text/html");
        response.setStatus(200);
        response.out() << file.rdbuf();
    }

private:
    std::string filePath_;
};

MainApp::MainApp(const Wt::WEnvironment& env, DatabaseManager& dbManager, UserSession& userSession) : Wt::WApplication(env), dbManager_(dbManager), userSession_(userSession) {
    setTitle("Pool It");

    if (!userSession_.isLoggedIn()) {
        doJavaScript("window.location.href='/frontend/login.html';");
        std::cout << "Not Logged in" << std::endl;
    }
    else {
        std::cout << "User Logged in" << std::endl;
    }
}

void static setupStaticFileHosting(Wt::WServer& server) {
    auto indexResource = std::make_shared<StaticFileResource>("frontend/index.html");
    server.addResource(indexResource, "/home");
}

int main(int argc, char** argv) {
    std::map<std::string, std::string> env = loadEnvFile("creds.env");
    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

        DatabaseManager dbManager(env["DB_PASSWORD"]);
        
        UserSession userSession(dbManager);

        server.addEntryPoint(Wt::EntryPointType::Application,
            [&dbManager, &userSession](const Wt::WEnvironment& env) {
                return std::make_unique<MainApp>(env, dbManager, userSession);
            });

        setupStaticFileHosting(server);

        auto signupHandler = std::make_shared<Signup>(dbManager, userSession);
        server.addResource(signupHandler, "/api/signup");

        auto loginHandler = std::make_shared<Login>(dbManager, userSession);
        server.addResource(loginHandler, "/api/login");

        auto rideUpdateHandler = std::make_shared<RideUpdate>(dbManager, userSession);
        server.addResource(rideUpdateHandler, "/api/rideUpdate");

        auto logoutHandler = std::make_shared<Logout>(userSession);
        server.addResource(logoutHandler, "/api/logout");


        server.run();

    }
    catch (const Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
