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

void static setupStaticFileHosting(Wt::WServer& server) {
    auto indexResource = std::make_shared<StaticFileResource>("frontend/index.html");
    server.addResource(indexResource, "/home");
}

int main(int argc, char** argv) {
    std::map<std::string, std::string> env = loadEnvFile("creds.env");
    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

        Wt::Dbo::Session session;
        connectDatabase(env["DB_PASSWORD"]);
        
        UserSession userSession(session);

        server.addEntryPoint(Wt::EntryPointType::Application,
            [](const Wt::WEnvironment& env) {
                return std::make_unique<MainApp>(env);
            });

        setupStaticFileHosting(server);

        auto signupHandler = std::make_shared<Signup>(session);
        server.addResource(signupHandler, "/api/signup");

        

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
