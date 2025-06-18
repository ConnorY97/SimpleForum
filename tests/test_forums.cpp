#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "sharedData/SharedServiceFixture.h"

TEST_CASE_METHOD(SharedServiceFixture, "Create a new forum successfully", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id = 1;
    bool result = forumService.createForum("Test Title", "Test Description", "Test User", id, error);

    REQUIRE(result);
    REQUIRE(error.empty());

    Forum forum;
    result = forumService.getForumById(id, forum, error);
    REQUIRE(result);
    REQUIRE(forum.title == "Test Title");
    REQUIRE(forum.description == "Test Description");
    REQUIRE(forum.createdBy == "Test User");
}

TEST_CASE_METHOD(SharedServiceFixture, "List multiple forums", "[ForumService]")
{
    std::string error;
    int forumAmount = 5;

    REQUIRE(forumService.clearForums(error));

    for (int i = 0; i < forumAmount; i++)
    {
        int forumId;
        bool result = forumService.createForum("Title " + std::to_string(i), "Description", "User", forumId, error);
        REQUIRE(result);
    }

    std::vector<Forum> forums;
    bool result = forumService.listForums(forums, error);
    REQUIRE(result);
    REQUIRE(forums.size() == forumAmount);
}

TEST_CASE_METHOD(SharedServiceFixture, "List forums when empty", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    std::vector<Forum> forums;
    bool result = forumService.listForums(forums, error);

    REQUIRE(result);
    REQUIRE(forums.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Get forum by valid ID", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id;
    REQUIRE(forumService.createForum("Title", "Desc", "User", id, error));

    Forum forum;
    REQUIRE(forumService.getForumById(id, forum, error));
    REQUIRE(forum.id == id);
}

TEST_CASE_METHOD(SharedServiceFixture, "Get forum by invalid ID returns error", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id;
    REQUIRE(forumService.createForum("Title", "Desc", "User", id, error));

    Forum forum;
    REQUIRE_FALSE(forumService.getForumById(id + 99, forum, error));
    REQUIRE(error == "Forum not found");
}

TEST_CASE_METHOD(SharedServiceFixture, "Update forum with correct ID and user", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id;
    REQUIRE(forumService.createForum("Old Title", "Old Desc", "TestUser", id, error));

    REQUIRE(forumService.updateForumById(id, "TestUser", "New Title", "New Desc", error));

    Forum updated;
    REQUIRE(forumService.getForumById(id, updated, error));
    REQUIRE(updated.title == "New Title");
    REQUIRE(updated.description == "New Desc");
}

TEST_CASE_METHOD(SharedServiceFixture, "Update forum with wrong username fails", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id;
    REQUIRE(forumService.createForum("Title", "Desc", "CorrectUser", id, error));

    REQUIRE_FALSE(forumService.updateForumById(id, "WrongUser", "Should Not Work", "Nope", error));
    REQUIRE_FALSE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Update non-existent forum fails", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    REQUIRE_FALSE(forumService.updateForumById(999, "User", "Title", "Desc", error));
    REQUIRE_FALSE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Creating a forum with empty title or description fails gracefully", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id;
    REQUIRE_FALSE(forumService.createForum("", "Desc", "User", id, error));
    REQUIRE_FALSE(error.empty());

    REQUIRE_FALSE(forumService.createForum("Title", "", "User", id, error));
    REQUIRE_FALSE(error.empty());
}

TEST_CASE_METHOD(SharedServiceFixture, "Duplicate forums are allowed if titles are the same", "[ForumService]")
{
    std::string error;
    REQUIRE(forumService.clearForums(error));

    int id1, id2;
    REQUIRE(forumService.createForum("SameTitle", "Desc1", "User1", id1, error));
    REQUIRE(forumService.createForum("SameTitle", "Desc2", "User2", id2, error));

    REQUIRE(id1 != id2);
}
