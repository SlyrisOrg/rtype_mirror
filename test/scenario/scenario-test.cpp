//
// Created by doom on 21/12/17.
//

#include <gtest/gtest.h>
#include <rtype/scenario/Scenario.hpp>

TEST(Scenario, Basic)
{
    scenario::Action action{};

    action.add<scenario::Music>(rtype::Configuration::Music::CitadelInstrumentalAmb);
    ASSERT_FALSE(!action);
    action.execute();
}

TEST(Scenario, Parsing)
{
    gutils::EventManager evtMgr{};
    // Scenario scenario scenario to be sure.
    scenario::Scenario scenario("assets/config/level/level1.json", evtMgr);
    ASSERT_EQ(scenario.action.get<scenario::Music>()->ID, rtype::Configuration::Music::CitadelInstrumentalAmb);
    ASSERT_EQ(scenario.action.get<scenario::Spawn>()->ID, "Monster01");
    ASSERT_EQ(scenario.action.get<scenario::Wait>()->duration, 0.4f);
    ASSERT_EQ(scenario.action.get<scenario::Spawn>()->ID, "Monster02");
    scenario.action.execute();
    ASSERT_EQ(scenario.action.get<scenario::Spawn>()->ID, "Boss01");
}

TEST(Scenario, execute)
{
    gutils::EventManager evtMgr{};
    scenario::Scenario scenario("assets/config/level/level1.json", evtMgr);
    while (!scenario.action)
    {
        scenario.action.execute();
    }
}

