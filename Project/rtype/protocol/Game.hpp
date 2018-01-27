//
// Created by doom on 24/01/18.
//

#ifndef RTYPE_PROTOCOL_GAME_HPP
#define RTYPE_PROTOCOL_GAME_HPP

#include <rtype/protocol/Protocol.hpp>
#include <meta/List.hpp>

namespace game
{
    struct Welcome
    {
        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap();
        }

        reflect_class(Welcome);
    };

    struct CreatePlayer
    {
        std::string factionName;
        std::string nickName;
        std::string scriptName;
        std::string scriptSelfName;
        std::string scriptTableName;
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

        reflect_class(CreatePlayer);
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

        reflect_class(Move);
    };

    struct Shoot
    {
        std::string ettName;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Shoot::ettName));
        }

        reflect_class(Shoot);
    };

    struct Collide
    {
        std::string first;
        std::string second;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Collide::first), reflect_member(&Collide::second));
        }

        reflect_class(Collide);
    };

    struct Kill
    {
        std::string victim;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Kill::victim));
        }

        reflect_class(Kill);
    };

    struct MatchStarted
    {
        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap();
        }

        reflect_class(MatchStarted);
    };

    struct Authenticate
    {
        std::string authToken;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Authenticate::authToken));
        }

        reflect_class(Authenticate);
    };

    using Packets = meta::TypeList
        <
            Welcome,
            MatchStarted,
            Authenticate
        >;
}

#endif //RTYPE_PROTOCOL_GAME_HPP
