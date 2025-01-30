#ifndef MAINAPP_H
#define MAINAPP_H

#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/WEnvironment.h>
#include <Wt/WResource.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

class MainApp : public Wt::WApplication {
public:
    MainApp(const Wt::WEnvironment& env);
};

// Class to handle button click request
class ButtonClickHandler : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;
};

#endif
