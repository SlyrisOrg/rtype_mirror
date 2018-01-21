//
// Created by doom on 18/01/18.
//

#ifndef RTYPE_SERVER_MATCHMAKING_HPP
#define RTYPE_SERVER_MATCHMAKING_HPP

#include <rtype/protocol/Protocol.hpp>
#include <meta/List.hpp>

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

    enum Mode
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

    using Packets = meta::TypeList<Authenticate, QueueJoin, QueueLeave>;
}

#endif //RTYPE_SERVER_MATCHMAKING_HPP
