#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "sharedData/SharedServiceFixture.h"
#include "../src/utils/CommentUtils.h"

TEST_CASE_METHOD(SharedServiceFixture, "Create a new comment", "[CommentService]")
{
	int forumId = 1;
	int commentId = 2;
	std::string user = "Test";
	std::string error;

	bool result = commentService.addComment(forumId, user, "This is a new comment", commentId, error);

	REQUIRE(result);
	REQUIRE(error.empty());
}


TEST_CASE_METHOD(SharedServiceFixture, "List Comments", "[CommentService]")
{
	std::string error;
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

/*
TEST_CASE_METHOD(SharedServiceFixture, "Get comment by ID", "[CommentServce]")
{
	std::string error;

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

TEST_CASE_METHOD(SharedServiceFixture, "Edit a comment", "[CommentServices]")
{
	std::string error;
	int forumId;
	bool result = forumService.createForum("CommentTest", "This is testing comments", "Test", forumId, error);

	REQUIRE(result);

	int commentId;
	std::string originalComment = "Test comment";
	std::string user = "TestUser";
	result = commentService.addComment(forumId, user, originalComment, commentId, error);

	REQUIRE(result);
	REQUIRE(error.empty());

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
*/