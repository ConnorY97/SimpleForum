#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/services/ForumService.h"

TEST_CASE("Create a new forum successfully", "[ForumService]")
{
	ForumService forumService;
	std::string error;

	int id = 1;
	REQUIRE(forumService.clearForums(error));
	bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

	REQUIRE(result);
	REQUIRE(error.empty());
}