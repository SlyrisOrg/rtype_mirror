//
// Created by szterg_r on 22/10/2017.
//

#include <rtype/error/RTypeError.hpp>

const detail::RtypeErrorCategory &login_error_category()
{
    static detail::RtypeErrorCategory c;
    return c;
}