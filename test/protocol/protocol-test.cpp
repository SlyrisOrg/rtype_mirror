//
// Created by doom on 21/12/17.
//

#include <gtest/gtest.h>
#include <serialization/Serialization.hpp>
#include <meta/List.hpp>
#include <protocol/Protocol.hpp>

namespace packets
{
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
}

using Packets = meta::TypeList<packets::Disconnect, packets::Lala>;
using Formatter = proto::Formatter<Packets>;
using Unformatter = proto::Unformatter<Packets>;

TEST(Protocol, IDs)
{
    ASSERT_EQ((proto::details::getID<Packets, packets::Disconnect>()), 0u);
}

TEST(Protocol, Disconnect)
{
    Formatter fmt;
    packets::Disconnect d{1, "Quit"};

    fmt.serialize(d);
    auto buf = fmt.extract();
    ASSERT_EQ(buf.size(), 8 + 4 + 8 + 4);

    Unformatter ufmt;
    auto packetVariant = ufmt.unserialize(buf);
    ASSERT_TRUE(std::holds_alternative<packets::Disconnect>(packetVariant));
    const packets::Disconnect &ref = std::get<packets::Disconnect>(packetVariant);
    ASSERT_EQ(ref.id, d.id);
    ASSERT_EQ(ref.reason, d.reason);
}

TEST(Protocol, Lala)
{
    Formatter fmt;
    packets::Lala d{123, 3.14f};

    fmt.serialize(d);
    auto buf = fmt.extract();
    ASSERT_EQ(buf.size(), 8 + 4 + 4);

    Unformatter ufmt;
    auto packetVariant = ufmt.unserialize(buf);
    ASSERT_TRUE(std::holds_alternative<packets::Lala>(packetVariant));
    const packets::Lala &ref = std::get<packets::Lala>(packetVariant);
    ASSERT_EQ(ref.lol, d.lol);
    ASSERT_EQ(ref.tralala, d.tralala);
}
