#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/services/CommentService.h"

TEST_CASE("Create a new comment successfully", "[CommentService]")
{
	CommentService commentService;
	std::string error;

	REQUIRE(commentService.clearComments(error));

	int forumId = 1;
	std::string user = "Test";

	bool result = commentService.addComment(forumId, user, "This is a new comment", error);

	REQUIRE(result);
	REQUIRE(error.empty());
}