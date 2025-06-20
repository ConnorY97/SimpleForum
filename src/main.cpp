#include "crow.h"
#include "routes/HomeRoute.h"
#include "routes/RegisterRoute.h"
#include "routes/LoginRoute.h"
#include "routes/LogoutRoute.h"
#include "routes/CreateForumRoute.h"
#include "routes/ListForumRoute.h"
#include "routes/ViewForumRoute.h"
#include "routes/ViewUsersRoute.h"
#include "routes/StaticRoutes.h"
#include "services/UserService.h"
#include "services/ForumService.h"
#include "services/CommentService.h"

int main() {
    crow::SimpleApp app;
    crow::mustache::set_base("templates");

    UserService userService;
    ForumService forumService;
    CommentService commentService;


    setUpStaticRoutes(app);
    setupHomeRoutes(app);
    setupRegisterRoutes(app, userService);
    setupLoginRoutes(app, userService);
    setupLogoutRoutes(app);
    setupCreateForumRoutes(app, forumService);
    setupForumListRoutes(app, forumService);
    setupForumViewRoutes(app, forumService, commentService);
    setupAdminRoutes(app, userService);

    app.port(18080).multithreaded().run();
}
