#include <Wt/WServer.h>
#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WResource.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <fstream>
#include <iostream>
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

MainApp::MainApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
    setTitle("Pool It");
    doJavaScript("window.location.href = '/home';");
}

void ButtonClickHandler::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) {
    response.setMimeType("text/plain");
    response.out() << "Button Clicked!";
}

void connectDatabase(std::string db_pass)
{
    Wt::Dbo::Session session;

    std::unique_ptr<Wt::Dbo::backend::MySQL> mysqlBackend =
        std::make_unique<Wt::Dbo::backend::MySQL>("poolit_db", "wt_poolit", db_pass, "127.0.0.1", 3306);

    session.setConnection(std::move(mysqlBackend));

    std::cout << "Connected to MySQL database successfully!" << std::endl;
}

void setupStaticFileHosting(Wt::WServer& server) {
    auto indexResource = std::make_shared<StaticFileResource>("frontend/index.html");
    server.addResource(indexResource, "/home");
}

int main(int argc, char** argv) {
    std::map<std::string, std::string> env = loadEnvFile("creds.env");
    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(Wt::EntryPointType::Application,
            [](const Wt::WEnvironment& env) {
                return std::make_unique<MainApp>(env);
            });

        setupStaticFileHosting(server);

        auto buttonHandler = std::make_shared<ButtonClickHandler>();
        server.addResource(buttonHandler, "/button-click");

        connectDatabase(env["DB_PASSWORD"]);

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
