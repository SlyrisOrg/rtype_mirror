//
// Created by szterg_r on 22/10/2017.
//

#ifndef RTYPE_LOGINERROR_HPP
#define RTYPE_LOGINERROR_HPP

#include <string>
#include <system_error>
#include <rtype/config/ErrorConfig.hpp>

enum class RTypeErrorCode
{
    Success = 0,
    NotFound = 101,
    InternalError = 102,
    InvalidSignature = 103,
    InvalidToken = 104,
    InvalidRequest = 105,
    UserPseudoEmpty = 201,
    UserPseudoBadFormat = 202,
    UserAlreadyTaken = 203,
    UserNameEmpty = 301,
    UserNameBadFormat = 302,
    UserEmailEmpty = 401,
    UserEmailBadFormat = 402,
    UserPasswordEmpty = 501,
    UserPasswordBadFormat = 502,
    UserGetDataFail = 601,
    UserPutDataFail = 701,
    UserPostDataFail = 801,
    UserSigningFail = 901,
    UserSignupFail = 1001,
    CredentialsEmpty = 1101,
    NoInternet = 2001
};

namespace std // NOLINT
{
    // Tell the C++ 11 STL metaprogramming that enum MathError::valid_errors
    // is registered with the standard error code system
    template <>
    struct is_error_code_enum<RTypeErrorCode> : std::true_type
    {
    };
}

namespace detail
{
    class RtypeErrorCategory : public std::error_category
    {
    public:

        const char *name() const noexcept final
        {
            return "RtypeErrorCategory";
        }

        std::string message(int c) const final
        {
            switch (static_cast<RTypeErrorCode>(c)) {
                case RTypeErrorCode::Success:
                    return cfg::error::tab[cfg::error::Success];
                case RTypeErrorCode::UserSigningFail:
                    return cfg::error::tab[cfg::error::UserSigningFail];
                case RTypeErrorCode::InternalError:
                case RTypeErrorCode::NotFound:
                    return cfg::error::tab[cfg::error::InternalError];
                case RTypeErrorCode::UserNameEmpty:
                    return cfg::error::tab[cfg::error::UserNameEmpty];
                case RTypeErrorCode::UserPseudoBadFormat:
                    return cfg::error::tab[cfg::error::UserPseudoBadFormat];
                case RTypeErrorCode::UserPasswordBadFormat:
                    return cfg::error::tab[cfg::error::UserPasswordBadFormat];
                case RTypeErrorCode::UserPasswordEmpty:
                    return cfg::error::tab[cfg::error::UserPasswordEmpty];
                case RTypeErrorCode::InvalidSignature:
                    return cfg::error::tab[cfg::error::InvalidSignature];
                case RTypeErrorCode::InvalidToken:
                    return cfg::error::tab[cfg::error::InvalidToken];
                case RTypeErrorCode::InvalidRequest:
                    return cfg::error::tab[cfg::error::InvalidRequest];
                case RTypeErrorCode::UserPseudoEmpty:
                    return cfg::error::tab[cfg::error::UserPseudoEmpty];
                case RTypeErrorCode::UserNameBadFormat:
                    return cfg::error::tab[cfg::error::UserNameBadFormat];
                case RTypeErrorCode::UserEmailEmpty:
                    return cfg::error::tab[cfg::error::UserEmailEmpty];
                case RTypeErrorCode::UserEmailBadFormat:
                    return cfg::error::tab[cfg::error::UserEmailBadFormat];
                case RTypeErrorCode::UserGetDataFail:
                    return cfg::error::tab[cfg::error::UserGetDataFail];
                case RTypeErrorCode::UserPutDataFail:
                    return cfg::error::tab[cfg::error::UserPutDataFail];
                case RTypeErrorCode::UserPostDataFail:
                    return cfg::error::tab[cfg::error::UserPostDataFail];
                case RTypeErrorCode::UserSignupFail:
                    return cfg::error::tab[cfg::error::UserSignupFail];
                case RTypeErrorCode::UserAlreadyTaken:
                    return cfg::error::tab[cfg::error::UserAlreadyTaken];
                case RTypeErrorCode::CredentialsEmpty:
                    return cfg::error::tab[cfg::error::CredentialsEmpty];
                case RTypeErrorCode::NoInternet:
                    return cfg::error::tab[cfg::error::NoInternet];
            }
            return "";
        }

        std::error_condition default_error_condition(int c) const noexcept final
        {
            switch (static_cast<RTypeErrorCode>(c)) {
                case RTypeErrorCode::UserNameEmpty:
                case RTypeErrorCode::UserPseudoBadFormat:
                case RTypeErrorCode::UserPasswordBadFormat:
                case RTypeErrorCode::UserPasswordEmpty:
                    return std::make_error_condition(std::errc::invalid_argument);
                default:
                    return std::error_condition{c, *this};
            }
        }
    };
}

extern const detail::RtypeErrorCategory &login_error_category();

inline std::error_code make_error_code(RTypeErrorCode e)
{
    return {static_cast<int>(e), login_error_category()};
}

#endif //RTYPE_LOGINERROR_HPP
