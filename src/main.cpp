#include "crow.h"

int main()
{
    crow::SimpleApp app;

    crow::mustache::set_base("templates");

    CROW_ROUTE(app, "/")([] {
        auto page = crow::mustache::load_text("fancypage.html");
        return page;
        });

    app.port(18080).multithreaded().run();

}