//
// Created by milerius on 14/12/17.
//

#ifndef RTYPE_LOADINGCONFIG_HPP
#define RTYPE_LOADINGCONFIG_HPP

#include <string>

namespace cfg::loading
{
    using namespace std::string_literals;

    //! Layouts
    inline std::string loadingLayout = "RTLoading.layout"s;

    //! Widget utils
    inline constexpr const unsigned int nbWidgets = 3;
}

#endif //RTYPE_LOADINGCONFIG_HPP
