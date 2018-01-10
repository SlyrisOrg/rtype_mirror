//
// Created by doom on 21/12/17.
//

#include <algorithm>
#include <gtest/gtest.h>
#include <rtype/components/Components.hpp>
#include <rtype/entity/EntityManager.hpp>
#include <rtype/utils/QuadTree.hpp>

using BBox = rtype::components::BoundingBox;
using Components = meta::TypeList<BBox>;
using EntityManager = game::EntityManager<Components>;
using Entity = EntityManager::Entity;

using namespace rtype;

TEST(QuadTree, BasicInsert)
{
    EntityManager em;

    Entity::ID id = em.createEntity();

    em[id].addComponent<BBox>(12, 12, 30, 30);

    rtype::QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    ASSERT_TRUE(quad.insert(id));
    QuadTree<EntityManager>::QuadNode *node = quad.getNode(id);
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(node->_AABB.height, node->_AABB.width));
    quad.remove(id);
}

TEST(QuadTree, FailedInsert)
{
    EntityManager em;

    Entity::ID id = em.createEntity();

    em[id].addComponent<BBox>(110, 12, 30, 30);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    ASSERT_FALSE(quad.insert(id));
}


TEST(QuadTree, MultipleInsert)
{
    EntityManager em;

    Entity::ID id1 = em.createEntity();
    Entity::ID id2 = em.createEntity();
    Entity::ID id3 = em.createEntity();
    Entity::ID id4 = em.createEntity();

    em[id1].addComponent<BBox>(12, 12, 10, 10);
    em[id2].addComponent<BBox>(20, 12, 30, 10);
    em[id3].addComponent<BBox>(31, 12, 10, 10);
    em[id4].addComponent<BBox>(44, 12, 10, 10);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    ASSERT_TRUE(quad.insert(id1));
    ASSERT_TRUE(quad.insert(id2));
    ASSERT_TRUE(quad.insert(id3));
    ASSERT_TRUE(quad.insert(id4));

    QuadTree<EntityManager>::QuadNode *nod = quad.getNode(id1);
    QuadTree<EntityManager>::QuadNode *nod2 = quad.getNode(id2);
    QuadTree<EntityManager>::QuadNode *nod3 = quad.getNode(id3);
    QuadTree<EntityManager>::QuadNode *nod4 = quad.getNode(id4);

    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod->_AABB.height, nod->_AABB.width));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod2->_AABB.height, nod2->_AABB.width));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod3->_AABB.height, nod3->_AABB.width));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod4->_AABB.height, nod4->_AABB.width));

    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
}

TEST(QuadTree, InsertAndSubDivide)
{
    EntityManager em;

    Entity::ID id1 = em.createEntity();
    Entity::ID id2 = em.createEntity();
    Entity::ID id3 = em.createEntity();
    Entity::ID id4 = em.createEntity();
    Entity::ID id5 = em.createEntity();
    Entity::ID id6 = em.createEntity();

    em[id1].addComponent<BBox>(10, 10, 10, 10);
    em[id2].addComponent<BBox>(30, 20, 30, 10);
    em[id3].addComponent<BBox>(10, 40, 10, 10);
    em[id4].addComponent<BBox>(40, 40, 10, 10);
    em[id5].addComponent<BBox>(80, 80, 10, 10);
    em[id6].addComponent<BBox>(40, 10, 10, 10);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    ASSERT_TRUE(quad.insert(id1));
    ASSERT_TRUE(quad.insert(id2));
    ASSERT_TRUE(quad.insert(id3));
    ASSERT_TRUE(quad.insert(id4));
    ASSERT_TRUE(quad.insert(id5));

    QuadTree<EntityManager>::QuadNode *nod = quad.getNode(id1);
    QuadTree<EntityManager>::QuadNode *nod2 = quad.getNode(id2);
    QuadTree<EntityManager>::QuadNode *nod3 = quad.getNode(id3);
    QuadTree<EntityManager>::QuadNode *nod4 = quad.getNode(id4);
    QuadTree<EntityManager>::QuadNode *nod5 = quad.getNode(id5);

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod->_AABB.left, nod->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod->_AABB.width, nod->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod5->_AABB.left, nod5->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod5->_AABB.width, nod5->_AABB.height));

    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));

    ASSERT_TRUE(quad.insert(id6));

    nod = quad.getNode(id1);
    nod2 = quad.getNode(id2);
    nod3 = quad.getNode(id3);
    nod4 = quad.getNode(id4);
    nod5 = quad.getNode(id5);
    QuadTree<EntityManager>::QuadNode *nod6 = quad.getNode(id6);

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod->_AABB.left, nod->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod->_AABB.width, nod->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 25), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod5->_AABB.left, nod5->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod5->_AABB.width, nod5->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 0), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));

    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_TRUE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
}

TEST(QuadTree, BasicRemove)
{
    EntityManager em;

    Entity::ID id = em.createEntity();

    em[id].addComponent<BBox>(12, 12, 30, 30);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    ASSERT_TRUE(quad.insert(id));

    QuadTree<EntityManager>::QuadNode *node = quad.getNode(id);

    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(node->_AABB.height, node->_AABB.width));

    quad.remove(id);

    ASSERT_TRUE(quad.getRoot()._items.empty());
}

TEST(QuadTree, RemoveAndUnDivide)
{
    EntityManager em;

    Entity::ID id1 = em.createEntity();
    Entity::ID id2 = em.createEntity();
    Entity::ID id3 = em.createEntity();
    Entity::ID id4 = em.createEntity();
    Entity::ID id5 = em.createEntity();
    Entity::ID id6 = em.createEntity();


    em[id1].addComponent<BBox>(10, 10, 10, 10);
    em[id2].addComponent<BBox>(30, 20, 10, 10);
    em[id3].addComponent<BBox>(10, 40, 10, 10);
    em[id4].addComponent<BBox>(40, 40, 10, 10);
    em[id5].addComponent<BBox>(80, 80, 10, 10);
    em[id6].addComponent<BBox>(40, 10, 10, 10);


    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    quad.insert(id1);
    quad.insert(id2);
    quad.insert(id3);
    quad.insert(id4);
    quad.insert(id5);
    quad.insert(id6);

    quad.remove(id1);

    QuadTree<EntityManager>::QuadNode *nod2 = quad.getNode(id2);
    QuadTree<EntityManager>::QuadNode *nod3 = quad.getNode(id3);
    QuadTree<EntityManager>::QuadNode *nod4 = quad.getNode(id4);
    QuadTree<EntityManager>::QuadNode *nod5 = quad.getNode(id5);
    QuadTree<EntityManager>::QuadNode *nod6 = quad.getNode(id6);

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod5->_AABB.left, nod5->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod5->_AABB.width, nod5->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));

    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));
}

TEST(QuadTree, RemoveAndUnDivideTwice)
{
    EntityManager em;

    Entity::ID id1 = em.createEntity();
    Entity::ID id2 = em.createEntity();
    Entity::ID id3 = em.createEntity();
    Entity::ID id4 = em.createEntity();
    Entity::ID id5 = em.createEntity();
    Entity::ID id6 = em.createEntity();

    em[id1].addComponent<BBox>(10, 10, 10, 10);
    em[id2].addComponent<BBox>(30, 20, 10, 10);
    em[id3].addComponent<BBox>(10, 40, 10, 10);
    em[id4].addComponent<BBox>(40, 40, 10, 10);
    em[id5].addComponent<BBox>(80, 80, 10, 10);
    em[id6].addComponent<BBox>(40, 10, 10, 10);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    quad.insert(id1);
    quad.insert(id2);
    quad.insert(id3);
    quad.insert(id4);
    quad.insert(id5);
    quad.insert(id6);

    quad.remove(id5);

    QuadTree<EntityManager>::QuadNode *nod = quad.getNode(id1);
    QuadTree<EntityManager>::QuadNode *nod2 = quad.getNode(id2);
    QuadTree<EntityManager>::QuadNode *nod3 = quad.getNode(id3);
    QuadTree<EntityManager>::QuadNode *nod4 = quad.getNode(id4);
    QuadTree<EntityManager>::QuadNode *nod6 = quad.getNode(id6);

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod->_AABB.left, nod->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod->_AABB.width, nod->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 25), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 0), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));

    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));

    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_TRUE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));

    quad.remove(id1);

    nod2 = quad.getNode(id2);
    nod3 = quad.getNode(id3);
    nod4 = quad.getNode(id4);
    nod6 = quad.getNode(id6);

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));

    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [id6](const auto &a) {
        return a == id6;
    })));
}

TEST(QuadTree, Move)
{
    EntityManager em;

    Entity::ID id1 = em.createEntity();
    Entity::ID id2 = em.createEntity();
    Entity::ID id3 = em.createEntity();
    Entity::ID id4 = em.createEntity();
    Entity::ID id5 = em.createEntity();
    Entity::ID id6 = em.createEntity();

    em[id1].addComponent<BBox>(10, 10, 10, 10);
    em[id2].addComponent<BBox>(30, 20, 10, 10);
    em[id3].addComponent<BBox>(10, 40, 10, 10);
    em[id4].addComponent<BBox>(40, 40, 10, 10);
    em[id5].addComponent<BBox>(80, 80, 10, 10);
    em[id6].addComponent<BBox>(40, 10, 10, 10);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 100, 100), em);

    quad.insert(id1);
    quad.insert(id2);
    quad.insert(id3);
    quad.insert(id4);
    quad.insert(id5);
    quad.insert(id6);

    em[id6].getComponent<BBox>().AABB.top = 35;
    ASSERT_TRUE(quad.move(id6));

    QuadTree<EntityManager>::QuadNode *nod = quad.getNode(id1);
    QuadTree<EntityManager>::QuadNode *nod2 = quad.getNode(id2);
    QuadTree<EntityManager>::QuadNode *nod3 = quad.getNode(id3);
    QuadTree<EntityManager>::QuadNode *nod4 = quad.getNode(id4);
    QuadTree<EntityManager>::QuadNode *nod6 = quad.getNode(id6);

    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id1](const auto &a) {
        return a == id1;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id2](const auto &a) {
        return a == id2;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id3](const auto &a) {
        return a == id3;
    })));
    ASSERT_TRUE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id4](const auto &a) {
        return a == id4;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [id5](const auto &a) {
        return a == id5;
    })));

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod->_AABB.left, nod->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod->_AABB.width, nod->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 25), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(25, 25), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));

    em[id1].getComponent<BBox>().AABB.left += 50;
    em[id2].getComponent<BBox>().AABB.left += 50;
    em[id3].getComponent<BBox>().AABB.left += 50;
    em[id6].getComponent<BBox>().AABB.left += 50;
    ASSERT_TRUE(quad.move(id1));
    ASSERT_TRUE(quad.move(id2));
    ASSERT_TRUE(quad.move(id3));
    ASSERT_TRUE(quad.move(id6));

    nod = quad.getNode(id1);
    nod2 = quad.getNode(id2);
    nod3 = quad.getNode(id3);
    nod4 = quad.getNode(id4);
    nod6 = quad.getNode(id6);

    ASSERT_EQ(sf::Vector2f(50, 0), sf::Vector2f(nod->_AABB.left, nod->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod->_AABB.width, nod->_AABB.height));
    ASSERT_EQ(sf::Vector2f(50, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(50, 0), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(50, 0), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(50, 50), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));
}

bool checkCol(std::vector<Entity::ID> &mayCollide, EntityManager &em)
{
    if (mayCollide.size() <= 1)
        return false;
    bool haveCollide = false;
    for (auto it = mayCollide.begin(); it != mayCollide.end() - 1; it++)
    {
        for (auto jt = it + 1; jt != mayCollide.end(); jt++)
        {
            if (em[*it].getComponent<BBox>().AABB.intersects(em[*jt].getComponent<BBox>().AABB))
            {
                haveCollide = true;
                //it->objPtr->doSomethingOnCollideWithObject(jt->objPtr->id);
                //jt->objPtr->doSomethingOnCollideWithObject(it->objPtr->id);
            }
        }
    }
    return haveCollide;
}

TEST(QuadTree, UseCase)
{
    // initializing the space

    EntityManager em;

    Entity::ID ship1 = em.createEntity();
    Entity::ID ship2 = em.createEntity();
    Entity::ID spaceThing1 = em.createEntity();
    Entity::ID spaceThing2 = em.createEntity();
    Entity::ID spaceThing3 = em.createEntity();
    Entity::ID spaceThing4 = em.createEntity();
    Entity::ID spaceThing5 = em.createEntity();
    Entity::ID spaceThing6 = em.createEntity();

    em[ship1].addComponent<BBox>(60, 30, 10, 10);
    em[ship2].addComponent<BBox>(165, 90, 10, 10);

    em[spaceThing1].addComponent<BBox>(145, 20, 5, 5);
    em[spaceThing2].addComponent<BBox>(130, 10, 5, 5);
    em[spaceThing3].addComponent<BBox>(180, 10, 5, 5);
    em[spaceThing4].addComponent<BBox>(190, 30, 5, 5);
    em[spaceThing5].addComponent<BBox>(180, 40, 5, 5);
    em[spaceThing6].addComponent<BBox>(70, 130, 5, 5);

    QuadTree<EntityManager> quad(sf::FloatRect(0, 0, 200, 200), em);

    ASSERT_TRUE(quad.insert(ship1));
    ASSERT_TRUE(quad.insert(ship2));
    ASSERT_TRUE(quad.insert(spaceThing1));
    ASSERT_TRUE(quad.insert(spaceThing2));
    ASSERT_TRUE(quad.insert(spaceThing3));
    ASSERT_TRUE(quad.insert(spaceThing4));
    ASSERT_TRUE(quad.insert(spaceThing5));
    ASSERT_TRUE(quad.insert(spaceThing6));

    // move some things and then check if moving things have collide
    // checking only space were moving things have occured,
    // you can't collide if no one moved around you

    em[ship1].getComponent<BBox>().AABB.left += 50;
    em[ship2].getComponent<BBox>().AABB.top -= 20;

    ASSERT_TRUE(quad.move(ship1)); // moved to an non-empty space but didn't collide
    ASSERT_TRUE(quad.move(ship2)); // moved to an empty space

    ASSERT_FALSE(checkCol(quad.getNode(ship1)->_items, em));
    ASSERT_FALSE(quad.getNode(ship1) == quad.getNode(ship2));
    ASSERT_FALSE(checkCol(quad.getNode(ship2)->_items, em));

    em[ship1].getComponent<BBox>().AABB.left += 50;
    em[ship2].getComponent<BBox>().AABB.top -= 35;

    ASSERT_TRUE(quad.move(ship1));
    ASSERT_TRUE(quad.move(ship2));
    // both ship moved to each other and collide

    ASSERT_TRUE(checkCol(quad.getNode(ship1)->_items, em));
    ASSERT_TRUE(quad.getNode(ship1) == quad.getNode(ship2)); // since true no need to check a second time for collision in this QuadNode

    // ending

}