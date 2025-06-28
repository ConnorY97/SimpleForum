#pragma once
#include "crow.h"
#include "../utils/LoggerUtilities.h"
#include <filesystem>
#include <vector>

std::string generateCardHTML(const std::string& filename, const std::string& imageUrl);

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

                std::string linksHtml;
                for (const auto& entry : fs::directory_iterator(contentDir))
                {
                    if (entry.is_regular_file())
                    {
                        auto path = entry.path();
                        if (path.extension() == ".html")
                        {
                            std::string filename = path.filename().string();

                            // Optional: check for associated image, e.g. same name .png
                            std::string baseName = filename.substr(0, filename.size() - 5);
                            std::string imagePath = "/public/images/" + baseName + ".png";

                            if (!fs::exists(fs::path("public/images") / (baseName + ".png")))
                                imagePath = ""; // No image found

                            linksHtml += generateCardHTML(filename, imagePath);
                        }
                    }
                }

                ctx["content_links"] = linksHtml;

                auto page = crow::mustache::load("portfolio.html").render(ctx);
                return crow::response{ page };
            });
}

std::string generateCardHTML(const std::string& filename, const std::string& imageUrl) {
    std::string displayName = filename.substr(0, filename.size() - 5); // Remove .html

    // You can add logic here to extract summaries or nicer titles if needed
    std::string link = "/public/content/" + filename;
    std::string cardHtml = "<a class=\"portfolio-card\" href=\"" + link + "\">";

    if (!imageUrl.empty()) {
        cardHtml += "<img src=\"" + imageUrl + "\" alt=\"" + displayName + " preview\">";
    }

    cardHtml += "<div class=\"card-content\">";
    cardHtml += "<h3>" + displayName + "</h3>";
    cardHtml += "<p>Click to view project details.</p>"; // You could extract summary later
    cardHtml += "</div></a>";

    return cardHtml;
}