#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/services/ForumService.h"
#include "../src/utils/ForumUtils.h"

TEST_CASE("Create a new forum successfully", "[ForumService]")
{
	ForumService forumService;
	std::string error;

	REQUIRE(forumService.clearForums(error));

	int id = 1;
	bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

	REQUIRE(result);
	REQUIRE(error.empty());
}

TEST_CASE("List forums", "[ForumService]")
{
	ForumService forumService;
	std::string error;
	int forumAmount = 5;

	REQUIRE(forumService.clearForums(error));

	for (int i = 0; i < forumAmount; i++)
	{
		int forumId;
		bool result = forumService.createForum("Test Title", "Test Description", "Test User", forumId, error);

		REQUIRE(result);
		REQUIRE(error.empty());
	}

	std::vector<Forum> forums;
	bool result = forumService.listForums(forums, error);

	REQUIRE(result);
	REQUIRE(error.empty());
	REQUIRE(forums.size() == forumAmount);
}

TEST_CASE("Get Forum by ID succeeds with correct ID", "[ForumService]")
{
	ForumService forumService;
	std::string error;

	int id = 1;
	REQUIRE(forumService.clearForums(error));
	bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	Forum forum;
	result = forumService.getForumById(id, forum, error);

	REQUIRE(result);
	REQUIRE(error.empty());
	REQUIRE(id == forum.id);
}

TEST_CASE("Get Forum by ID fails with incorrect id", "[ForumService]")
{
	ForumService forumService;
	std::string error;

	int id = 1;
	REQUIRE(forumService.clearForums(error));
	bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	Forum forum;
	result = forumService.getForumById(id + 1, forum, error);

	REQUIRE_FALSE(result);
	REQUIRE(error == "Forum not found");
	REQUIRE_FALSE(id == forum.id);
}

TEST_CASE("Update Forum with correct Id", "[ForumService]")
{
		ForumService forumService;
	std::string error;

	REQUIRE(forumService.clearForums(error));

	int id = 1;
	bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	result = forumService.updateForumById(id, "Test User", "New Title", "New description", error);

	REQUIRE(result);
	REQUIRE(error.empty());
}

TEST_CASE("Fail to update Forum with incorrect Id", "[ForumService]")
{
		ForumService forumService;
	std::string error;

	REQUIRE(forumService.clearForums(error));

	int id = 1;
	bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

	REQUIRE(result);
	REQUIRE(error.empty());

	int incorrectId = 2;
	result = forumService.updateForumById(incorrectId, "Test User", "New Title", "New description", error);

	REQUIRE_FALSE(result);
}