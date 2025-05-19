#include "crow.h"
#include "routes/HomeRoute.h"
#include "routes/RegisterRoute.h"
#include "routes/LoginRoute.h"
#include "services/UserService.h"

int main() {
    crow::SimpleApp app;
    crow::mustache::set_base("templates");

    UserService userService;

    setupHomeRoutes(app);
    setupRegisterRoutes(app, userService);
    setupLoginRoutes(app, userService);
    app.port(18080).multithreaded().run();
}
