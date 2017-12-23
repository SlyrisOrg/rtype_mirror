//
// Created by doom on 21/12/17.
//

#include <gtest/gtest.h>
#include <protocol/Protocol.hpp>

TEST(Protocol, IDs)
{
    ASSERT_EQ(proto::getID<proto::Disconnect>(), 0u);
}

TEST(Protocol, Disconnect)
{
    proto::Formatter fmt;
    proto::Disconnect d{1, "Quit"};

    fmt.serialize(d);
    auto buf = fmt.extract();
    ASSERT_EQ(buf.size(), 8 + 4 + 8 + 4);

    proto::Unformatter ufmt;
    auto packetVariant = ufmt.unserialize(buf);
    ASSERT_TRUE(std::holds_alternative<proto::Disconnect>(packetVariant));
    const proto::Disconnect &ref = std::get<proto::Disconnect>(packetVariant);
    ASSERT_EQ(ref.id, d.id);
    ASSERT_EQ(ref.reason, d.reason);
}

TEST(Protocol, Lala)
{
    proto::Formatter fmt;
    proto::Lala d{123, 3.14f};

    fmt.serialize(d);
    auto buf = fmt.extract();
    ASSERT_EQ(buf.size(), 8 + 4 + 4);

    proto::Unformatter ufmt;
    auto packetVariant = ufmt.unserialize(buf);
    ASSERT_TRUE(std::holds_alternative<proto::Lala>(packetVariant));
    const proto::Lala &ref = std::get<proto::Lala>(packetVariant);
    ASSERT_EQ(ref.lol, d.lol);
    ASSERT_EQ(ref.tralala, d.tralala);
}
