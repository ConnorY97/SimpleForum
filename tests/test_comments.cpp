#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/services/CommentService.h"
#include "../src/services/ForumService.h"
#include "../src/utils/CommentUtils.h"

TEST_CASE("Create a new comment", "[CommentService]")
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

TEST_CASE("List Comments", "[CommentService]")
{
	CommentService commentService;
	ForumService forumService;
	std::string error;

	REQUIRE(commentService.clearComments(error));
	REQUIRE(forumService.clearForums(error));

	int forumId;
	bool result = forumService.createForum("CommentTest", "This is testing comments", "Test", forumId, error);

	REQUIRE(result);

	result = commentService.addComment(forumId, "TestUser", "Test comment", error);

	REQUIRE(result);

	std::vector<Comment> comments;
	result = commentService.getCommentsForForum(forumId, comments, error);

	REQUIRE(result);
	REQUIRE(error.empty());
}