//
// Created by doom on 03/01/18.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <gtest/gtest.h>
#include <rtype/entity/ECS.hpp>
#include <rtype/systems/SCollision.hpp>

TEST(Components, BoundingBox)
{
    sf::RectangleShape shape{sf::Vector2f(30, 30)};
    shape.setPosition(125, 125);

    sf::Texture text;
    text.loadFromFile("assets/sprite/ShipInterior.png");
    sf::Sprite spr(text);
    spr.setPosition(0, 0);

    rtype::components::BoundingBox box{50, 50, 10, 10};
    rtype::components::BoundingBox box2{sf::Vector2f(0, 0), sf::Vector2f(500, 500)};
    rtype::components::BoundingBox box3{shape};
    rtype::components::BoundingBox box4{spr};

    ASSERT_EQ(box.AABB, sf::FloatRect(50, 50, 10, 10));
    ASSERT_EQ(box3.AABB, sf::FloatRect(125, 125, 30, 30));
    ASSERT_EQ(box2.AABB, sf::FloatRect(0, 0, 500, 500));
    ASSERT_EQ(box4.AABB, sf::FloatRect(0, 0, spr.getTexture()->getSize().x, spr.getTexture()->getSize().y));
}

TEST(Components, Sprite)
{
    sf::Texture text;
    text.loadFromFile("assets/sprite/ShipInterior.png");

    rtype::components::Sprite spr(text);
    ASSERT_EQ(spr.sprite.getTexture()->getSize(), sf::Vector2u(1920, 1080));
}

namespace rtc = rtype::components;

TEST(Components, MiniGame)
{
    rtype::EntityManager em;
    gutils::EventManager evtMgr;
    rtype::SCollision collision(evtMgr, em);
    rtype::Entity::ID spaceID = em.createEntity();

    auto space2ID = em.createEntity();

    sf::Texture text;
    text.loadFromFile("assets/sprite/ShipInterior.png");

    em[spaceID].addComponent<rtc::Sprite>(text);
    em[spaceID].addComponent<rtc::BoundingBox>(em[spaceID].getComponent<rtc::Sprite>().sprite);

    em[space2ID].addComponent<rtc::Sprite>(text, sf::IntRect(text.getSize().x, text.getSize().y, 125, 125));
    // We want to use the global bounds of the sprite :D
    em[space2ID].addComponent<rtc::BoundingBox>(em[space2ID].getComponent<rtc::Sprite>().sprite);


    // First the texture is 1920, 1080, but we only want a square in so 125, 125 starting at 0,0, and we test the intersection
    // Between two game object, it's not pixel perfect because we use the sfml functions, but it's a proof.
    ASSERT_EQ(em[space2ID].getComponent<rtc::BoundingBox>().AABB, sf::FloatRect(0, 0, 125, 125));
    ASSERT_TRUE(collision.boundingBoxCheck(em[space2ID].getComponent<rtc::BoundingBox>(),
                                           em[spaceID].getComponent<rtc::BoundingBox>()));
}