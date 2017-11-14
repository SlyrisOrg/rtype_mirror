//
// Created by szterg_r on 29/10/2017.
//

#ifndef RTYPE_ERRORCONFIG_HPP
#define RTYPE_ERRORCONFIG_HPP

#include <array>
#include <string>

namespace cfg::error
{
    /**
     * @enum Code
     * @brief This enum is related to another one in RTypeErrorCode.hpp
     * This enumeration has values ​​starting at 0 unlike its counterpart in RTypeErrorCode
     * which is based on RTYPE API error codes. It is there to translate the error codes of the API
     * to a message readable by the player, or by the developer via a log message.
     * @see RTypeErrorCode.hpp
     */
    enum Code
    {
        Success = 0,
        NotFound = 1,
        InternalError = 2,
        InvalidSignature = 3,
        InvalidToken = 4,
        InvalidRequest = 5,
        UserPseudoEmpty = 6,
        UserPseudoBadFormat = 7,
        UserAlreadyTaken = 8,
        UserNameEmpty = 9,
        UserNameBadFormat = 10,
        UserEmailEmpty = 11,
        UserEmailBadFormat = 12,
        UserPasswordEmpty = 13,
        UserPasswordBadFormat = 14,
        UserGetDataFail = 15,
        UserPutDataFail = 16,
        UserPostDataFail = 17,
        UserSigningFail = 18,
        UserSignupFail = 19,
        CredentialsEmpty = 20,
        NoInternet = 21
    };
    extern std::array<std::string, 22u> tab;
}


#endif //RTYPE_ERRORCONFIG_HPP
