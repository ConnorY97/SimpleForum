// SharedServiceFixture.h
#pragma once

#include "../src/services/CommentService.h"
#include "../src/services/ForumService.h"
#include "../src/services/UserService.h"
#include <catch2/catch_test_macros.hpp>

struct SharedServiceFixture
{
    SharedServiceFixture()
    {
        bool ok = userService.clearUsers(error);
        if (!ok)
            std::cerr << "Failed to clear users: " << error << std::endl;
        REQUIRE(ok);
        error.clear();

        ok = forumService.clearForums(error);
        if (!ok)
            std::cerr << "Failed to clear forums: " << error << std::endl;
        REQUIRE(ok);
        error.clear();

        ok = commentService.clearComments(error);
        if (!ok)
            std::cerr << "Failed to clear comments: " << error << std::endl;
        REQUIRE(ok);
        error.clear();
    }

    ~SharedServiceFixture() = default;

    UserService userService;
    ForumService forumService;
    CommentService commentService;
    std::string error;
};
