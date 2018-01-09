//
// Created by doom on 03/01/18.
//

#include <gtest/gtest.h>
#include <entity/EntityManager.hpp>

struct Box
{
    Box(unsigned int x, unsigned int y, unsigned int width, unsigned int height) noexcept :
        x(x), y(y), width(width), height(height)
    {
    }

    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

struct Lol
{
    int i{2};
    int j{3};
};

struct Lul
{
};

using Components = meta::TypeList<Box, Lol, Lul>;

using EntityManager = game::EntityManager<Components>;
using Entity = EntityManager::Entity;

TEST(ECS, EntityManager)
{
    EntityManager em;
    Entity::ID id = em.createEntity();

    ASSERT_FALSE(em[id].hasComponent<Box>());
    ASSERT_FALSE(em[id].hasComponent<Lol>());
    ASSERT_FALSE(em[id].hasComponent<Lul>());

    em[id].addComponent<Box>(1, 2, 3, 4);
    ASSERT_TRUE(em[id].hasComponent<Box>());
    Box &box = em[id].getComponent<Box>();
    ASSERT_EQ(box.x, 1u);
    ASSERT_EQ(box.y, 2u);
    ASSERT_EQ(box.width, 3u);
    ASSERT_EQ(box.height, 4u);

    em[id].addComponent<Lol>();
    ASSERT_TRUE(em[id].hasComponent<Box>());
    Box &box2 = em[id].getComponent<Box>();
    ASSERT_EQ(box2.x, 1u);
    ASSERT_EQ(box2.y, 2u);
    ASSERT_EQ(box2.width, 3u);
    ASSERT_EQ(box2.height, 4u);
    ASSERT_TRUE(em[id].hasComponent<Lol>());
    Lol &lol = em[id].getComponent<Lol>();
    ASSERT_EQ(lol.i, 2);
    ASSERT_EQ(lol.j, 3);

    em[id].removeComponent<Lol>();
    ASSERT_FALSE(em[id].hasComponent<Lol>());
    ASSERT_TRUE(em[id].hasComponent<Box>());
    Box &box3 = em[id].getComponent<Box>();
    ASSERT_EQ(box3.x, 1u);
    ASSERT_EQ(box3.y, 2u);
    ASSERT_EQ(box3.width, 3u);
    ASSERT_EQ(box3.height, 4u);
}

TEST(ECS, SimpleForEach)
{
    EntityManager em;

    for (size_t i = 0; i < 10; ++i) {
        auto id = em.createEntity();
        em[id].addComponent<Lol>();
    }
    size_t nbMatched = 0;
    em.for_each<Lol>([&nbMatched](auto &) {
        nbMatched += 1;
    });
    ASSERT_EQ(nbMatched, 10u);

    nbMatched = 0;
    em.for_each<Box>([&nbMatched](auto &) {
        nbMatched += 1;
    });
    ASSERT_EQ(nbMatched, 0u);
}

TEST(ECS, MultipleForEach)
{
    EntityManager em;

    for (size_t i = 0; i < 10; ++i) {
        auto id = em.createEntity();
        em[id].addComponent<Lol>();
        em[id].addComponent<Box>(1, 2, 3, 4);
    }
    size_t nbMatched = 0;
    em.for_each<Lol, Box>([&nbMatched](auto &) {
        nbMatched += 1;
    });
    ASSERT_EQ(nbMatched, 10u);

    nbMatched = 0;
    em.for_each<Box, Lul>([&nbMatched](auto &) {
        nbMatched += 1;
    });
    ASSERT_EQ(nbMatched, 0u);
}

TEST(ECS, Sweeping)
{
    EntityManager em;

    for (size_t i = 0; i < 10; ++i) {
        auto id = em.createEntity();
        if (i % 2)
            em[id].addComponent<Lol>();
        em[id].addComponent<Lul>();
    }

    em.for_each<Lol>([](auto &et) {
        et.mark();
    });
    em.sweepEntities();
    ASSERT_EQ(em.nbEntities(), 5u);
}

//Tiny std::allocator-conformant allocator :D
template <typename T>
struct CustomAllocator : std::allocator<T>
{
};

using CustomAllocEntityMgr = game::EntityManager<Components, CustomAllocator>;
using CAEntity = CustomAllocEntityMgr::Entity;

TEST(ECS, CustomAllocator)
{
    CustomAllocEntityMgr em;
    CAEntity::ID id = em.createEntity();

    ASSERT_FALSE(em[id].hasComponent<Box>());
    ASSERT_FALSE(em[id].hasComponent<Lol>());
    ASSERT_FALSE(em[id].hasComponent<Lul>());

    em[id].addComponent<Box>(1, 2, 3, 4);
    ASSERT_TRUE(em[id].hasComponent<Box>());
    Box &box = em[id].getComponent<Box>();
    ASSERT_EQ(box.x, 1u);
    ASSERT_EQ(box.y, 2u);
    ASSERT_EQ(box.width, 3u);
    ASSERT_EQ(box.height, 4u);

    em[id].addComponent<Lol>();
    ASSERT_TRUE(em[id].hasComponent<Box>());
    Box &box2 = em[id].getComponent<Box>();
    ASSERT_EQ(box2.x, 1u);
    ASSERT_EQ(box2.y, 2u);
    ASSERT_EQ(box2.width, 3u);
    ASSERT_EQ(box2.height, 4u);
    ASSERT_TRUE(em[id].hasComponent<Lol>());
    Lol &lol = em[id].getComponent<Lol>();
    ASSERT_EQ(lol.i, 2);
    ASSERT_EQ(lol.j, 3);

    em[id].removeComponent<Lol>();
    ASSERT_FALSE(em[id].hasComponent<Lol>());
    ASSERT_TRUE(em[id].hasComponent<Box>());
    Box &box3 = em[id].getComponent<Box>();
    ASSERT_EQ(box3.x, 1u);
    ASSERT_EQ(box3.y, 2u);
    ASSERT_EQ(box3.width, 3u);
    ASSERT_EQ(box3.height, 4u);
}

TEST(ECS, StructuredBinding)
{
    EntityManager em;
    Entity::ID id = em.createEntity();

    em[id].addComponent<Box>(1, 2, 3, 4);
    em[id].addComponent<Lol>();
    const auto &[box, lol] = em[id].getComponents<Box, Lol>();
    ASSERT_TRUE((std::is_same_v<decltype(box), Box &>));
    ASSERT_TRUE((std::is_same_v<decltype(lol), Lol &>));
}