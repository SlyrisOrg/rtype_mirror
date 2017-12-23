//
// Created by doom on 22/12/17.
//

#ifndef RTYPE_PROTOCOL_PACKETS_HPP
#define RTYPE_PROTOCOL_PACKETS_HPP

#include <meta/List.hpp>
#include <serialization/Serialization.hpp>

namespace proto
{
    /**
     * Packets are tiny structures matching the serialization::is_serializable type trait.
     *
     * New packets can be added by creating new structures and appending them to the Packets
     * TypeList defined below. The protocol classes should handle the rest for you, however
     * writing tests is a good way to ensure that everything works as expected.
     */

    using ClientID = unsigned int;

    struct Disconnect
    {
        ClientID id;
        std::string reason;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Disconnect::id),
                                 reflect_member(&Disconnect::reason));
        }
    };

    struct Lala
    {
        int lol;
        float tralala;

        static constexpr auto serializableFields() noexcept
        {
            return meta::makeMap(reflect_member(&Lala::lol),
                                 reflect_member(&Lala::tralala));
        }
    };

    using Packets = meta::TypeList<Disconnect, Lala>;

    template <typename T>
    static inline constexpr size_t getID() noexcept
    {
        return meta::list::Position<Packets, T>();
    }
}

#endif //RTYPE_PROTOCOL_PACKETS_HPP
