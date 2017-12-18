//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/ErrorConfig.hpp>

namespace cfg::error
{
    std::array<std::string, 22u> tab =
        {
            {
                "Success.",
                "Internal error occurred, please contact an administrator.",
                "Internal error occurred, please contact an administrator.",
                "Error invalid signature.",
                "Error invalid Token.",
                "Error invalid request.",
                "Error user pseudo is empty.",
                "Error user pseudo bad format.",
                "This nickname is already taken by another user.",
                "Error username is empty.",
                "Error username bad format.",
                "Error password is empty.",
                "Error password bad format.",
                "Error user get data fail",
                "Error user put data fail.",
                "Error user post data fail.",
                "Username or password is incorrect. Please try logging in again.",
                "Error user signup fail.",
                "Username or email is missing",
                "Oops, it seems you do not have internet"
            }
        };
}