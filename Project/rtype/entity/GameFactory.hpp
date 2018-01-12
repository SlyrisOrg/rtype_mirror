//
// Created by milerius on 09/01/18.
//

#ifndef RTYPE_GAMEFACTORY_HPP
#define RTYPE_GAMEFACTORY_HPP

#include <SFML/Graphics/Texture.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/config/Configuration.hpp>

namespace rtype
{
    class GameFactory
    {
    public:
        static void setEntityManager(EntityManager *ettMgr) noexcept
        {
            _ettMgr = ettMgr;
        }

        static Entity::ID createSpaceShip(Entity::ID spaceID,
                                          sfutils::Animation &anim,
                                          sf::Rect<int> &boundingBox,
                                          sf::Vector2f pos) noexcept
        {
            auto &ettMgr = *_ettMgr;
            auto &animComponent = ettMgr[spaceID].addComponent<rtc::Animation>(&anim,
                                                                               sfutils::AnimatedSprite::Playing,
                                                                               sf::seconds(0.04f),
                                                                               false);
            ettMgr[spaceID].getComponent<rtc::Animation>().anim.setPosition(pos);
            sf::Vector2f position{static_cast<float>(animComponent.anim.getPosition().x + boundingBox.left),
                                  static_cast<float>(animComponent.anim.getPosition().y + boundingBox.top)};
            sf::Vector2f size{static_cast<float>(boundingBox.width), static_cast<float>(boundingBox.height)};
            ettMgr[spaceID].addComponent<rtc::BoundingBox>(position, size, boundingBox);
            ettMgr[spaceID].addComponent<rtc::Movement>();
            return spaceID;
        }

        template <typename ...Args>
        static Entity::ID createPlayerSpaceShip(Args &&...args) noexcept
        {
            Entity::ID spaceID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            ettMgr[spaceID].addComponent<rtc::Player>();
            return createSpaceShip(spaceID, std::forward<Args>(args)...);
        }

        static Entity::ID
        createBullet(sf::Texture &texture, const sf::FloatRect &AABB, Configuration::SoundEffect eff) noexcept
        {
            Entity::ID bulletID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            auto &ett = ettMgr[bulletID];
            sf::Sprite &sprite = ett.addComponent<rtc::Sprite>(texture).sprite;
            sprite.setPosition(
                sf::Vector2f{AABB.left + AABB.width,
                             AABB.top + AABB.height / 2.f -
                             ettMgr[bulletID].getComponent<rtc::Sprite>().sprite.getGlobalBounds().height /
                             2.f});
            ettMgr[bulletID].addComponent<rtc::BoundingBox>(ettMgr[bulletID].getComponent<rtc::Sprite>().sprite);
            ettMgr[bulletID].addComponent<rtc::Bullet>();
            ettMgr[bulletID].addComponent<rtc::SoundEffect>(eff);
            return bulletID;
        }

    private:
        static EntityManager *_ettMgr;
    };
}

#endif //RTYPE_GAMEFACTORY_HPP
