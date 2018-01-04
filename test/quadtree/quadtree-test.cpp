//
// Created by doom on 21/12/17.
//

#include <algorithm>
#include <gtest/gtest.h>
#include <rtype/utils/QuadTree.hpp>

struct Thing
{
    sf::Vector2f position;
    sf::Vector2f size;

    size_t id;
};

TEST(QuadTree, BasicInsert)
{
    Thing obj{sf::Vector2f(12, 12), sf::Vector2f(30, 30), 0};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    ASSERT_TRUE(quad.insert(&obj));
    rtype::QuadTree<Thing>::QuadNode *node = quad.getNode(obj.id);
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(node->_AABB.height, node->_AABB.width));
    quad.remove(&obj);
}

TEST(QuadTree, FailedInsert)
{
    Thing obj{sf::Vector2f(110, 12), sf::Vector2f(30, 30), 0};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    ASSERT_FALSE(quad.insert(&obj));
}


TEST(QuadTree, MultipleInsert)
{
    Thing obj{sf::Vector2f(12, 12), sf::Vector2f(10, 10), 0};
    Thing obj2{sf::Vector2f(20, 12), sf::Vector2f(10, 10), 1};
    Thing obj3{sf::Vector2f(31, 12), sf::Vector2f(10, 10), 2};
    Thing obj4{sf::Vector2f(44, 12), sf::Vector2f(10, 10), 3};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    ASSERT_TRUE(quad.insert(&obj));
    ASSERT_TRUE(quad.insert(&obj2));
    ASSERT_TRUE(quad.insert(&obj3));
    ASSERT_TRUE(quad.insert(&obj4));

    rtype::QuadTree<Thing>::QuadNode *nod = quad.getNode(obj.id);
    rtype::QuadTree<Thing>::QuadNode *nod2 = quad.getNode(obj2.id);
    rtype::QuadTree<Thing>::QuadNode *nod3 = quad.getNode(obj3.id);
    rtype::QuadTree<Thing>::QuadNode *nod4 = quad.getNode(obj4.id);

    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod->_AABB.height, nod->_AABB.width));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod2->_AABB.height, nod2->_AABB.width));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod3->_AABB.height, nod3->_AABB.width));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod4->_AABB.height, nod4->_AABB.width));

    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
}

TEST(QuadTree, InsertAndSubDivide)
{
    Thing obj{sf::Vector2f(10, 10), sf::Vector2f(10, 10), 0};
    Thing obj2{sf::Vector2f(30, 20), sf::Vector2f(10, 10), 1};
    Thing obj3{sf::Vector2f(10, 40), sf::Vector2f(10, 10), 2};
    Thing obj4{sf::Vector2f(40, 40), sf::Vector2f(10, 10), 3};
    Thing obj5{sf::Vector2f(80, 80), sf::Vector2f(10, 10), 4};
    Thing obj6{sf::Vector2f(40, 10), sf::Vector2f(10, 10), 5};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    ASSERT_TRUE(quad.insert(&obj));
    ASSERT_TRUE(quad.insert(&obj2));
    ASSERT_TRUE(quad.insert(&obj3));
    ASSERT_TRUE(quad.insert(&obj4));
    ASSERT_TRUE(quad.insert(&obj5));

    rtype::QuadTree<Thing>::QuadNode *nod = quad.getNode(obj.id);
    rtype::QuadTree<Thing>::QuadNode *nod2 = quad.getNode(obj2.id);
    rtype::QuadTree<Thing>::QuadNode *nod3 = quad.getNode(obj3.id);
    rtype::QuadTree<Thing>::QuadNode *nod4 = quad.getNode(obj4.id);
    rtype::QuadTree<Thing>::QuadNode *nod5 = quad.getNode(obj5.id);

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

    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_TRUE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));

    ASSERT_TRUE(quad.insert(&obj6));

    nod = quad.getNode(obj.id);
    nod2 = quad.getNode(obj2.id);
    nod3 = quad.getNode(obj3.id);
    nod4 = quad.getNode(obj4.id);
    nod5 = quad.getNode(obj5.id);
    rtype::QuadTree<Thing>::QuadNode *nod6 = quad.getNode(obj6.id);

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

    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod5->_items.begin(), nod5->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_TRUE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
}

TEST(QuadTree, BasicRemove)
{
    Thing obj{sf::Vector2f(12, 12), sf::Vector2f(30, 30), 0};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    ASSERT_TRUE(quad.insert(&obj));

    rtype::QuadTree<Thing>::QuadNode *node = quad.getNode(obj.id);

    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(node->_AABB.height, node->_AABB.width));

    quad.remove(&obj);

    ASSERT_TRUE(quad.getRoot()._items.empty());
}

TEST(QuadTree, RemoveAndUnDivide)
{
    Thing obj{sf::Vector2f(10, 10), sf::Vector2f(10, 10), 0};
    Thing obj2{sf::Vector2f(30, 20), sf::Vector2f(10, 10), 1};
    Thing obj3{sf::Vector2f(10, 40), sf::Vector2f(10, 10), 2};
    Thing obj4{sf::Vector2f(40, 40), sf::Vector2f(10, 10), 3};
    Thing obj5{sf::Vector2f(80, 80), sf::Vector2f(10, 10), 4};
    Thing obj6{sf::Vector2f(40, 10), sf::Vector2f(10, 10), 5};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    quad.insert(&obj);
    quad.insert(&obj2);
    quad.insert(&obj3);
    quad.insert(&obj4);
    quad.insert(&obj5);
    quad.insert(&obj6);

    quad.remove(&obj);

    rtype::QuadTree<Thing>::QuadNode *nod2 = quad.getNode(obj2.id);
    rtype::QuadTree<Thing>::QuadNode *nod3 = quad.getNode(obj3.id);
    rtype::QuadTree<Thing>::QuadNode *nod4 = quad.getNode(obj4.id);
    rtype::QuadTree<Thing>::QuadNode *nod5 = quad.getNode(obj5.id);
    rtype::QuadTree<Thing>::QuadNode *nod6 = quad.getNode(obj6.id);

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

    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));
}

TEST(QuadTree, RemoveAndUnDivideTwice)
{
    Thing obj{sf::Vector2f(10, 10), sf::Vector2f(10, 10), 0};
    Thing obj2{sf::Vector2f(30, 20), sf::Vector2f(10, 10), 1};
    Thing obj3{sf::Vector2f(10, 40), sf::Vector2f(10, 10), 2};
    Thing obj4{sf::Vector2f(40, 40), sf::Vector2f(10, 10), 3};
    Thing obj5{sf::Vector2f(80, 80), sf::Vector2f(10, 10), 4};
    Thing obj6{sf::Vector2f(40, 10), sf::Vector2f(10, 10), 5};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    quad.insert(&obj);
    quad.insert(&obj2);
    quad.insert(&obj3);
    quad.insert(&obj4);
    quad.insert(&obj5);
    quad.insert(&obj6);

    quad.remove(&obj5);

    rtype::QuadTree<Thing>::QuadNode *nod = quad.getNode(obj.id);
    rtype::QuadTree<Thing>::QuadNode *nod2 = quad.getNode(obj2.id);
    rtype::QuadTree<Thing>::QuadNode *nod3 = quad.getNode(obj3.id);
    rtype::QuadTree<Thing>::QuadNode *nod4 = quad.getNode(obj4.id);
    rtype::QuadTree<Thing>::QuadNode *nod6 = quad.getNode(obj6.id);

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

    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod->_items.begin(), nod->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod3->_items.begin(), nod3->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod4->_items.begin(), nod4->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));

    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_TRUE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));

    quad.remove(&obj);

    nod2 = quad.getNode(obj2.id);
    nod3 = quad.getNode(obj3.id);
    nod4 = quad.getNode(obj4.id);
    nod6 = quad.getNode(obj6.id);

    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod2->_AABB.left, nod2->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod2->_AABB.width, nod2->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod3->_AABB.left, nod3->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod3->_AABB.width, nod3->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod4->_AABB.left, nod4->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod4->_AABB.width, nod4->_AABB.height));
    ASSERT_EQ(sf::Vector2f(0, 0), sf::Vector2f(nod6->_AABB.left, nod6->_AABB.top));
    ASSERT_EQ(sf::Vector2f(100, 100), sf::Vector2f(nod6->_AABB.width, nod6->_AABB.height));

    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
    })));
    ASSERT_TRUE((std::any_of(nod2->_items.begin(), nod2->_items.end(), [obj6](const auto &a) {
        return a.objPtr->id == obj6.id;
    })));
}

TEST(QuadTree, Move)
{
    Thing obj{sf::Vector2f(10, 10), sf::Vector2f(10, 10), 0};
    Thing obj2{sf::Vector2f(30, 20), sf::Vector2f(10, 10), 1};
    Thing obj3{sf::Vector2f(10, 40), sf::Vector2f(10, 10), 2};
    Thing obj4{sf::Vector2f(40, 40), sf::Vector2f(10, 10), 3};
    Thing obj5{sf::Vector2f(80, 80), sf::Vector2f(10, 10), 4};
    Thing obj6{sf::Vector2f(40, 10), sf::Vector2f(10, 10), 5};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 100, 100));

    quad.insert(&obj);
    quad.insert(&obj2);
    quad.insert(&obj3);
    quad.insert(&obj4);
    quad.insert(&obj5);
    quad.insert(&obj6);

    obj6.position.y = 35;
    ASSERT_TRUE(quad.move(&obj6));

    rtype::QuadTree<Thing>::QuadNode *nod = quad.getNode(obj.id);
    rtype::QuadTree<Thing>::QuadNode *nod2 = quad.getNode(obj2.id);
    rtype::QuadTree<Thing>::QuadNode *nod3 = quad.getNode(obj3.id);
    rtype::QuadTree<Thing>::QuadNode *nod4 = quad.getNode(obj4.id);
    rtype::QuadTree<Thing>::QuadNode *nod6 = quad.getNode(obj6.id);

    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj](const auto &a) {
        return a.objPtr->id == obj.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj2](const auto &a) {
        return a.objPtr->id == obj2.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj3](const auto &a) {
        return a.objPtr->id == obj3.id;
    })));
    ASSERT_TRUE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj4](const auto &a) {
        return a.objPtr->id == obj4.id;
    })));
    ASSERT_FALSE((std::any_of(nod6->_items.begin(), nod6->_items.end(), [obj5](const auto &a) {
        return a.objPtr->id == obj5.id;
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

    obj.position.x += 50;
    obj2.position.x += 50;
    obj3.position.x += 50;
    obj6.position.x += 50;
    ASSERT_TRUE(quad.move(&obj));
    ASSERT_TRUE(quad.move(&obj2));
    ASSERT_TRUE(quad.move(&obj3));
    ASSERT_TRUE(quad.move(&obj6));

    nod = quad.getNode(obj.id);
    nod2 = quad.getNode(obj2.id);
    nod3 = quad.getNode(obj3.id);
    nod4 = quad.getNode(obj4.id);
    nod6 = quad.getNode(obj6.id);

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

template <typename T>
bool checkCol(std::vector<T> &mayCollide)
{
    if (mayCollide.size() <= 1)
        return false;
    bool haveCollide = false;
    for (auto it = mayCollide.begin(); it != mayCollide.end() - 1; it++)
    {
        for (auto jt = it + 1; jt != mayCollide.end(); jt++)
        {
            if (it->rect.getGlobalBounds().intersects(jt->rect.getGlobalBounds()))
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

    Thing ship1{sf::Vector2f(60, 30), sf::Vector2f(10, 10), 0};
    Thing ship2{sf::Vector2f(165, 90), sf::Vector2f(10, 10), 1};
    Thing spaceThing1{sf::Vector2f(145, 20), sf::Vector2f(5, 5), 2};
    Thing spaceThing2{sf::Vector2f(130, 10), sf::Vector2f(5, 5), 3};
    Thing spaceThing3{sf::Vector2f(180, 10), sf::Vector2f(5, 5), 4};
    Thing spaceThing4{sf::Vector2f(190, 30), sf::Vector2f(5, 5), 5};
    Thing spaceThing5{sf::Vector2f(180, 40), sf::Vector2f(5, 5), 6};
    Thing spaceThing6{sf::Vector2f(70, 130), sf::Vector2f(5, 5), 7};

    rtype::QuadTree<Thing> quad(sf::FloatRect(0, 0, 200, 200));

    ASSERT_TRUE(quad.insert(&ship1));
    ASSERT_TRUE(quad.insert(&ship2));
    ASSERT_TRUE(quad.insert(&spaceThing1));
    ASSERT_TRUE(quad.insert(&spaceThing2));
    ASSERT_TRUE(quad.insert(&spaceThing3));
    ASSERT_TRUE(quad.insert(&spaceThing4));
    ASSERT_TRUE(quad.insert(&spaceThing5));
    ASSERT_TRUE(quad.insert(&spaceThing6));

    // move some things and then check if moving things have collide
    // checking only space were moving things have occured,
    // you can't collide if no one moved around you

    ship1.position.x += 50;
    ship2.position.y -= 20;

    ASSERT_TRUE(quad.move(&ship1)); // moved to an non-empty space but didn't collide
    ASSERT_TRUE(quad.move(&ship2)); // moved to an empty space

    ASSERT_FALSE(checkCol(quad.getNode(ship1.id)->_items));
    ASSERT_FALSE(quad.getNode(ship1.id) == quad.getNode(ship2.id));
    ASSERT_FALSE(checkCol(quad.getNode(ship2.id)->_items));

    ship1.position.x += 50;
    ship2.position.y -= 35;

    ASSERT_TRUE(quad.move(&ship1));
    ASSERT_TRUE(quad.move(&ship2));
    // both ship moved to each other and collide

    ASSERT_TRUE(checkCol(quad.getNode(ship1.id)->_items));
    ASSERT_TRUE(quad.getNode(ship1.id) == quad.getNode(ship2.id)); // since true no need to check a second time for collision in this QuadNode

    // ending

}