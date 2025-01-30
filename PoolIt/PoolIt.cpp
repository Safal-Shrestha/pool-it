#include <Wt/WServer.h>
#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WResource.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <fstream>
#include <iostream>

// Class to handle serving the index.html file
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

        // Serve index.html with correct MIME type
        response.setMimeType("text/html");
        response.setStatus(200);
        response.out() << file.rdbuf();
    }

private:
    std::string filePath_;
};

// MainApp class (for backend logic in the future)
class MainApp : public Wt::WApplication {
public:
    explicit MainApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
        setTitle("Pool It");
        // Backend logic can be added later
    }
};

// Function to set up static file hosting for index.html
void setupStaticFileHosting(Wt::WServer& server) {
    auto indexResource = std::make_shared<StaticFileResource>("frontend/index.html");
    server.addResource(indexResource, "/");
    server.addResource(indexResource, "/index.html");
}

int main(int argc, char** argv) {
    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

        setupStaticFileHosting(server);

        /*server.addEntryPoint(Wt::EntryPointType::Application,
            [](const Wt::WEnvironment& env) { return std::make_unique<MainApp>(env); });*/

        server.run();  // Ensure the server keeps running
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

