//
// Created by milerius on 22/01/18.
//

#include <gtest/gtest.h>
#include <rtype/lua/LuaManager.hpp>

TEST(ECS, LuaTest)
{
    using namespace std::string_view_literals;
    rtype::EntityManager em;
    em.createEntity();
    em.createEntity();
    auto fid = em.createEntity();
    auto id = em.createEntity();
    em[id].addComponent<rtype::components::BoundingBox>(sf::Vector2f{1500.f, 2500.f}, sf::Vector2f{500.f, 500.f});
    em[id].addComponent<rtype::components::Lua>("test.lua", "test");
    em[fid].addComponent<rtype::components::BoundingBox>(sf::Vector2f{500.f, 500.f}, sf::Vector2f{500.f, 500.f});
    rtype::lua::LuaManager luaMgr(em, fs::current_path() /=  "assets/scripts/");
    luaMgr.loadAll();
    float res = luaMgr["bar"]();
    ASSERT_EQ(res, 300);
}