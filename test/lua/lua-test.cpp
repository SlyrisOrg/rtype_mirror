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
    em[id].addComponent<rtype::components::Lua>("test.lua", "test");
    rtype::lua::LuaManager luaMgr{em, fs::current_path() /=  "assets/scripts/"};
    luaMgr.loadAll();
    float res = luaMgr.executeFunction<float>("bar");
    std::string str = luaMgr.executeFunction<std::string>("foo", "salut");
    luaMgr.executeFunction("affz");
    ASSERT_EQ(str, "lol");
    ASSERT_EQ(res, 300);
}