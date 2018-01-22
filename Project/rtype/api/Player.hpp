//
// Created by roman sztergbaum on 15/01/2018.
//

#ifndef RTYPE_SERVER_PLAYER_HPP
#define RTYPE_SERVER_PLAYER_HPP

#include <string>
#include <utils/Enums.hpp>
#include <serialization/Serialization.hpp>

namespace rtype
{
    ENUM(Faction,
         Bheet,
         Kooy,
         Maul);

    struct Player
    {
        using FactionT = Faction::EnumType;

        bool hasAuthToken() const noexcept
        {
            return !authToken.empty();
        }

        std::string authToken;
        std::string nickName;
        float xp;
        unsigned int gold;
        unsigned int lvl;
        FactionT faction;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Player::authToken),
                                 reflect_member(&Player::nickName),
                                 reflect_member(&Player::xp),
                                 reflect_member(&Player::gold),
                                 reflect_member(&Player::lvl),
                                 reflect_member(&Player::faction));
        }
    };
}

#endif //RTYPE_SERVER_PLAYER_HPP
