//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/ErrorConfig.hpp>

namespace cfg::error
{
    using namespace std::string_literals;
    inline std::array<std::string, 22u> tab =
        {
            {
                "Success."s,
                "Internal error occurred, please contact an administrator."s,
                "Internal error occurred, please contact an administrator."s,
                "Error invalid signature."s,
                "Error invalid Token."s,
                "Error invalid request."s,
                "Error user pseudo is empty."s,
                "Error user pseudo bad format."s,
                "This nickname is already taken by another user."s,
                "Error username is empty."s,
                "Error username bad format."s,
                "Error password is empty."s,
                "Error password bad format."s,
                "Error user get data fail"s,
                "Error user put data fail."s,
                "Error user post data fail."s,
                "Username or password is incorrect. Please try logging in again."s,
                "Error user signup fail."s,
                "Username or email is missing"s,
                "Oops, it seems you do not have internet"s
            }
        };
}