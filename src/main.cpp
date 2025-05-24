#include "crow.h"
#include "routes/HomeRoute.h"
#include "routes/RegisterRoute.h"
#include "routes/LoginRoute.h"
#include "routes/LogoutRoute.h"
#include "services/UserService.h"

int main() {
    crow::SimpleApp app;
    crow::mustache::set_base("templates");

    UserService userService;

    setupHomeRoutes(app);
    setupRegisterRoutes(app, userService);
    setupLoginRoutes(app, userService);
    setupLogoutRoutes(app);
    app.port(18080).multithreaded().run();
}
