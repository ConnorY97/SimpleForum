#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "sharedData/SharedServiceFixture.h"
#include "../src/utils/CommentUtils.h"

TEST_CASE_METHOD(SharedServiceFixture, "Create a new comment and verify fields", "[CommentService]")
{
	std::string error;
	REQUIRE(forumService.clearForums(error));
	REQUIRE(commentService.clearComments(error));

	int forumId;
	REQUIRE(forumService.createForum("Test Forum", "Desc", "User", forumId, error));

	int commentId;
	std::string user = "TestUser";
	std::string text = "This is a new comment";
	REQUIRE(commentService.addComment(forumId, user, text, commentId, error));
	REQUIRE(error.empty());

	Comment c;
	REQUIRE(commentService.getCommentById(commentId, c, error));
	REQUIRE(c.forumId == forumId);
	REQUIRE(c.username == user);
	REQUIRE(c.comment == text);
}

TEST_CASE_METHOD(SharedServiceFixture, "List comments for a forum", "[CommentService]")
{
	std::string error;
	REQUIRE(forumService.clearForums(error));
	REQUIRE(commentService.clearComments(error));

	int forumId;
	REQUIRE(forumService.createForum("Forum", "Description", "Test", forumId, error));

	for (int i = 0; i < 3; i++)
	{
		int commentId;
		REQUIRE(commentService.addComment(forumId, "User", "Comment #" + std::to_string(i), commentId, error));
	}

	std::vector<Comment> comments;
	REQUIRE(commentService.getCommentsForForum(forumId, comments, error));
	REQUIRE(comments.size() == 3);
}

TEST_CASE_METHOD(SharedServiceFixture, "Get comment by ID", "[CommentService]")
{
	std::string error;
	REQUIRE(forumService.clearForums(error));
	REQUIRE(commentService.clearComments(error));

	int forumId;
	REQUIRE(forumService.createForum("Forum", "Desc", "User", forumId, error));

	int commentId;
	std::string comment = "Some text";
	REQUIRE(commentService.addComment(forumId, "User", comment, commentId, error));

	Comment c;
	REQUIRE(commentService.getCommentById(commentId, c, error));
	REQUIRE(c.comment == comment);
}

TEST_CASE_METHOD(SharedServiceFixture, "Edit a comment successfully", "[CommentService]")
{
	std::string error;
	REQUIRE(forumService.clearForums(error));
	REQUIRE(commentService.clearComments(error));

	int forumId;
	REQUIRE(forumService.createForum("Forum", "Desc", "User", forumId, error));

	int commentId;
	std::string original = "Original comment";
	std::string user = "User";
	REQUIRE(commentService.addComment(forumId, user, original, commentId, error));

	std::string updated = "Updated comment";
	REQUIRE(commentService.updateCommentById(commentId, user, updated, error));

	Comment c;
	REQUIRE(commentService.getCommentById(commentId, c, error));
	REQUIRE(c.comment == updated);
}

TEST_CASE_METHOD(SharedServiceFixture, "Fail to edit comment with wrong user", "[CommentService]")
{
	std::string error;
	REQUIRE(forumService.clearForums(error));
	REQUIRE(commentService.clearComments(error));

	int forumId;
	REQUIRE(forumService.createForum("Forum", "Desc", "User", forumId, error));

	int commentId;
	REQUIRE(commentService.addComment(forumId, "CorrectUser", "Text", commentId, error));

	std::string wrongUser = "WrongUser";
	bool result = commentService.updateCommentById(commentId, wrongUser, "Should fail", error);
	REQUIRE_FALSE(result);
	REQUIRE_FALSE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Fail to get nonexistent comment by ID", "[CommentService]")
{
	std::string error;
	REQUIRE(commentService.clearComments(error));

	Comment c;
	REQUIRE_FALSE(commentService.getCommentById(99999, c, error));
	REQUIRE(error == "Comment not found");
}

TEST_CASE_METHOD(SharedServiceFixture, "Fail to update nonexistent comment", "[CommentService]")
{
	std::string error;
	REQUIRE(commentService.clearComments(error));

	std::string user = "User";
	bool result = commentService.updateCommentById(99999, user, "Update", error);
	REQUIRE_FALSE(result);
	REQUIRE_FALSE(error.empty());
}

//TEST_CASE_METHOD(SharedServiceFixture, "Adding empty comment fails", "[CommentService]")
//{
//	std::string error;
//	REQUIRE(forumService.clearForums(error));
//	REQUIRE(commentService.clearComments(error));
//
//	int forumId;
//	REQUIRE(forumService.createForum("Forum", "Desc", "User", forumId, error));
//
//	int commentId;
//	bool result = commentService.addComment(forumId, "User", "", commentId, error);
//	REQUIRE_FALSE(result);
//	REQUIRE_FALSE(error.empty());
//}
