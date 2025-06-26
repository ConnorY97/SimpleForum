#pragma once
#include "crow.h"
#include "../utils/LoggerUtilities.h"
#include <filesystem>
#include <vector>

inline void setupPortfolioRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/portfolio")
        ([]
            {
                namespace fs = std::filesystem;

                crow::mustache::context ctx;

                std::vector<std::string> contentLinks;
                const std::string contentDir = "public/content";

                for (const auto& entry : fs::directory_iterator(contentDir))
                {
                    if (entry.is_regular_file())
                    {
                        auto path = entry.path();
                        if (path.extension() == ".html")
                        {
                            std::string filename = path.filename().string();
                            std::string displayName = filename.substr(0, filename.size() - 5); // strip .html

                            // Optionally, you can prettify displayName here, e.g. capitalize or replace _ with spaces

                            std::string link = "<a href=\"/public/content/" + filename + "\">" + displayName + "</a>";
                            contentLinks.push_back(link);
                        }
                    }
                }

                // Join links with <br> or list items
                std::string linksHtml;
                for (const auto& link : contentLinks)
                {
                    linksHtml += link + "<br>";
                }

                ctx["content_links"] = linksHtml;

                auto page = crow::mustache::load("portfolio.html").render(ctx);
                return crow::response{ page };
            });
}
