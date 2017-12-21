//
// Created by zouz on 21/12/17.
//

#ifndef RTYPE_LOBBYCONFIG_HPP
#define RTYPE_LOBBYCONFIG_HPP

#include <rtype/scenes/private/ForwardEnumLobby.hpp>

namespace cfg::lobby
{
    static inline constexpr const char *lobbyLayout = "RTQueue.layout"; //TODO Change layout ?

    //! Widget utils
    static inline constexpr const unsigned int nbWidgets = rtype::LobbyWidgets::size();
}

#endif //RTYPE_LOBBYCONFIG_HPP
