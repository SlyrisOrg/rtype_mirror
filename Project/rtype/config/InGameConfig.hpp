//
// Created by milerius on 02/01/18.
//

#ifndef RTYPE_INGAMECONFIG_HPP
#define RTYPE_INGAMECONFIG_HPP

#include <rtype/scenes/private/ForwardEnumInGame.hpp>

namespace cfg::play
{
    //! Layouts
    static inline constexpr const char *UILayout = "RTUI.layout";

    //! Utils
    static inline constexpr const unsigned int nbWidgets = rtype::UIWidgets::size();
}

#endif //RTYPE_INGAMECONFIG_HPP
