//
// Created by doom on 18/01/18.
//

#ifndef RTYPE_SERVER_MATCHMAKING_HPP
#define RTYPE_SERVER_MATCHMAKING_HPP

#include <rtype/protocol/Protocol.hpp>
#include <meta/List.hpp>
#include <rtype/api/Player.hpp>

namespace matchmaking
{
    struct Authenticate
    {
        std::string authToken;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Authenticate::authToken));
        }
    };

    enum Mode : unsigned int
    {
        Solo,
        Duo,
        Trio,
        Quatuor,
        None,
        NbModes = None,
    };

    struct QueueJoin
    {
        Mode mode;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&QueueJoin::mode));
        }
    };

    struct QueueLeave
    {
        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap();
        }
    };

    struct QueueStarted
    {
        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap();
        }
    };

    struct MatchFound
    {
        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap();
        }
    };

    struct GameHostInfo
    {
        unsigned short port;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&GameHostInfo::port));
        }
    };

    using PlayerInfo = rtype::Player;

    using Packets = meta::TypeList
        <
            Authenticate,
            QueueJoin,
            QueueLeave,
            QueueStarted,
            MatchFound,
            PlayerInfo,
            GameHostInfo
        >;
}

#endif //RTYPE_SERVER_MATCHMAKING_HPP
