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
	int commentId;
	result = commentService.addComment(forumId, "TestUser", "Test comment", commentId, error);

	REQUIRE(result);

	std::vector<Comment> comments;
	result = commentService.getCommentsForForum(forumId, comments, error);

	REQUIRE(result);
	REQUIRE(error.empty());
}

TEST_CASE("Get comment by ID", "[CommentServce]")
{
	CommentService commentService;
	ForumService forumService;
	std::string error;

	REQUIRE(commentService.clearComments(error));
	REQUIRE(forumService.clearForums(error));

	int forumId;
	bool result = forumService.createForum("CommentTest", "This is testing comments", "Test", forumId, error);

	REQUIRE(result);

	int commentId;
	std::string comment = "Test comment";
	result = commentService.addComment(forumId, "TestUser", comment, commentId, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	Comment testComment;
	result = commentService.getCommentById(commentId, testComment, error);

	REQUIRE(result);
	REQUIRE(error.empty());
	REQUIRE(testComment.comment == comment);
}

TEST_CASE("Edit a comment", "[CommentServices]")
{
	CommentService commentService;
	ForumService forumService;
	std::string error;

	REQUIRE(commentService.clearComments(error));
	REQUIRE(forumService.clearForums(error));

	int forumId;
	bool result = forumService.createForum("CommentTest", "This is testing comments", "Test", forumId, error);

	REQUIRE(result);

	int commentId;
	std::string originalComment = "Test comment";
	result = commentService.addComment(forumId, "TestUser", originalComment, commentId, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	std::string user;
	std::string updateComment = "Updated comment";
	result = commentService.updateCommentById(commentId, user, updateComment, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	Comment comment;
	result = commentService.getCommentById(commentId, comment, error);

	REQUIRE(result);
	REQUIRE(error.empty());
	REQUIRE(comment.comment == updateComment);
}