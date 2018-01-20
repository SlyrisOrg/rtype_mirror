//
// Created by milerius on 09/01/18.
//

#ifndef RTYPE_GAMEFACTORY_HPP
#define RTYPE_GAMEFACTORY_HPP

#include <random>
#include <SFML/Graphics/Texture.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/config/Configuration.hpp>
#include <rtype/utils/DemoUtils.hpp>

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
                                                                               sf::seconds(0.015f),
                                                                               false);
            ettMgr[spaceID].getComponent<rtc::Animation>().anim.setPosition(pos);
            sf::Vector2f position{static_cast<float>(animComponent.anim.getPosition().x + boundingBox.left),
                                  static_cast<float>(animComponent.anim.getPosition().y + boundingBox.top)};
            sf::Vector2f size{static_cast<float>(boundingBox.width), static_cast<float>(boundingBox.height)};
            ettMgr[spaceID].addComponent<rtc::BoundingBox>(position, size, boundingBox);
            ettMgr[spaceID].addComponent<rtc::Movement>();
            return spaceID;
        }

        template <typename Map, typename ...Args>
        static Entity::ID createPlayerSpaceShip(Map transitionMap, Args &&...args) noexcept
        {
            Entity::ID spaceID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            ettMgr[spaceID].addComponent<rtc::Player>(transitionMap);
            ettMgr[spaceID].addComponent<rtc::GameFieldLayer>();
            return createSpaceShip(spaceID, std::forward<Args>(args)...);
        }

        static Entity::ID createBullet(sf::Texture &texture,
                                       const sf::FloatRect &AABB,
                                       Configuration::SoundEffect eff) noexcept
        {
            Entity::ID bulletID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            auto &ett = ettMgr[bulletID];
            sf::Sprite &sprite = ett.addComponent<rtc::Sprite>(texture).sprite;
            sprite.setPosition(sf::Vector2f{AABB.left + AABB.width,
                               AABB.top + AABB.height / 2.f
                               - ettMgr[bulletID].getComponent<rtc::Sprite>().sprite.getGlobalBounds().height / 2.f});
            ettMgr[bulletID].addComponent<rtc::BoundingBox>(ettMgr[bulletID].getComponent<rtc::Sprite>().sprite);
            ettMgr[bulletID].addComponent<rtc::Bullet>();
            ettMgr[bulletID].addComponent<rtc::SoundEffect>(eff);
            ettMgr[bulletID].addComponent<rtc::GameFieldLayer>();
            return bulletID;
        }

        static Entity::ID createFieldItem(sf::Texture &texture,
                                          const sf::Vector2f &pos,
                                          float speed,
                                          float scale,
                                          [[maybe_unused]] char alpha,
                                          rtc::LayerType layer,
                                          demo::field::SubField *subField,
                                          unsigned int ItemPackIndex) noexcept
        {
            Entity ::ID itemID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            ettMgr[itemID].addComponent<rtc::FieldItem>(subField, ItemPackIndex);
            sf::Sprite &sprite = ettMgr[itemID].addComponent<rtc::Sprite>(texture).sprite;
            sprite.setPosition(pos);
            sprite.scale(scale, scale);
            ettMgr[itemID].addComponent<rtc::Speed>(speed);
            switch(layer)
            {
                case rtc::LayerType::StarField :
                    ettMgr[itemID].addComponent<rtc::StarFieldLayer>();
                    break;
                case rtc::LayerType::Fog1 :
                    ettMgr[itemID].addComponent<rtc::Fog1Layer>();
                    break;
                case rtc::LayerType::Planet1 :
                    ettMgr[itemID].addComponent<rtc::Planet1Layer>();
                    break;
                case rtc::LayerType::Fog2 :
                    ettMgr[itemID].addComponent<rtc::Fog2Layer>();
                    break;
                case rtc::LayerType::Planet2 :
                    ettMgr[itemID].addComponent<rtc::Planet2Layer>();
                    break;
                case rtc::LayerType::GameField :
                    ettMgr[itemID].addComponent<rtc::GameFieldLayer>();
                    break;
                case rtc::LayerType::Fog3 :
                    ettMgr[itemID].addComponent<rtc::Fog3Layer>();
                    break;
                default:
                    ettMgr[itemID].addComponent<rtc::StarFieldLayer>();
            }
            return itemID;
        }

    private:
        static EntityManager *_ettMgr;
    };
}

#endif //RTYPE_GAMEFACTORY_HPP
