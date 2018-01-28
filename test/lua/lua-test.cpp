//
// Created by milerius on 22/01/18.
//

#include <gtest/gtest.h>
#include <rtype/lua/LuaManager.hpp>
#include <thread>

TEST(Lua, LuaTest)
{
    using namespace std::string_view_literals;
    rtype::EntityManager em;
    auto id = em.createEntity();
    em[id].addComponent<rtype::components::BoundingBox>(sf::Vector2f{1500.f, 2500.f}, sf::Vector2f{500.f, 500.f});
    em[id].addComponent<rtype::components::Lua>("test.lua", "test", "testTable");
    rtype::lua::LuaManager luaMgr{em, fs::current_path() /= "assets/scripts/"};
    luaMgr.loadAll();
    float res = luaMgr.executeFunction<float>("bar");
    std::string str = luaMgr.executeFunction<std::string>("foo", "salut");
    luaMgr.executeFunction("affz");
    ASSERT_EQ(str, "lol");
    ASSERT_EQ(res, 300);
}

TEST(Lua, LuaCall)
{
    using namespace std::string_view_literals;
    rtype::EntityManager em;

    auto id = em.createEntity();
    em[id].addComponent<rtype::components::BoundingBox>(sf::Vector2f{1500.f, 2500.f}, sf::Vector2f{500.f, 500.f});
    em[id].addComponent<rtype::components::Lua>("test.lua", "test", "testTable");

    auto id2 = em.createEntity();
    em[id2].addComponent<rtype::components::BoundingBox>(sf::Vector2f{1500.f, 2500.f}, sf::Vector2f{500.f, 500.f});
    em[id2].addComponent<rtype::components::Lua>("test2.lua", "test2", "testTable2");

    rtype::lua::LuaManager luaMgr{em, fs::current_path() /= "assets/scripts/"};
    luaMgr.loadAll();

    em.for_each<rtype::components::BoundingBox, rtype::components::Lua>([&luaMgr](rtype::Entity &ett) {
        auto &tableName = ett.getComponent<rtype::components::Lua>().tableName;
        luaMgr[tableName]["update"](ett.getID(), 1);
    });
}