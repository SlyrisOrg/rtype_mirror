//
// Created by milerius on 26/01/18.
//

#ifndef RTYPE_GAMEPACKETS_HPP
#define RTYPE_GAMEPACKETS_HPP

#include <meta/Map.hpp>
#include <meta/Meta.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/components/Components.hpp>

namespace game
{
    struct CreatePlayer
    {
        std::string  factionName;
        std::string  nickName;
        std::string  scriptName;
        std::string  scriptSelfName;
        std::string  scriptTableName;
        float posX;
        float posY;
        int width;
        int height;
        int left;
        int top;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&CreatePlayer::factionName),
                                 reflect_member(&CreatePlayer::nickName),
                                 reflect_member(&CreatePlayer::scriptName),
                                 reflect_member(&CreatePlayer::scriptSelfName),
                                 reflect_member(&CreatePlayer::scriptTableName),
                                 reflect_member(&CreatePlayer::posX),
                                 reflect_member(&CreatePlayer::posY),
                                 reflect_member(&CreatePlayer::width),
                                 reflect_member(&CreatePlayer::height),
                                 reflect_member(&CreatePlayer::left),
                                 reflect_member(&CreatePlayer::top));
        }
    };

    struct Move
    {
        float x;
        float y;
        std::string ettName;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Move::x), reflect_member(&Move::y), reflect_member(&Move::ettName));
        }
    };

    struct Shoot
    {
        std::string ettName;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Shoot::ettName));
        }
    };

    struct Collide
    {
        std::string first;
        std::string second;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Collide::first), reflect_member(&Collide::second));
        }
    };

    struct Kill
    {
        std::string victim;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Kill::victim));
        }
    };

    using Packets = meta::TypeList
        <
            CreatePlayer,
            Move,
            Shoot,
            Collide,
            Kill
        >;
}

#endif //RTYPE_GAMEPACKETS_HPP
