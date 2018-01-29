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
#include <rtype/utils/QuadTree.hpp>
#include <rtype/lua/LuaManager.hpp>

namespace rtype
{
    class GameFactory
    {
    public:
        static void setEntityManager(EntityManager *ettMgr) noexcept
        {
            _ettMgr = ettMgr;
        }

        static void setQuadTree(QuadTree<EntityManager> *quadtree) noexcept
        {
            _quadtree = quadtree;
        }

        static void setLuaManager(lua::LuaManager *lua) noexcept
        {
            _lua = lua;
        }

        static Entity::ID createSpaceShip(Entity::ID spaceID,
                                          sfutils::Animation &anim,
                                          sf::Rect<int> &boundingBox,
                                          sf::Vector2f pos,
                                          rtc::Stat stat) noexcept
        {
            auto &ettMgr = *_ettMgr;
            ettMgr[spaceID].addComponent<rtc::Animation>(&anim,
                                                         sfutils::AnimatedSprite::Playing,
                                                         sf::seconds(0.015f),
                                                         false);
            ettMgr[spaceID].getComponent<rtc::Animation>().anim.setPosition(pos);
            sf::Vector2f position{static_cast<float>(pos.x + boundingBox.left),
                                  static_cast<float>(pos.y + boundingBox.top)};
            sf::Vector2f size{static_cast<float>(boundingBox.width), static_cast<float>(boundingBox.height)};
            ettMgr[spaceID].addComponent<rtc::BoundingBox>(position, size, boundingBox);
            ettMgr[spaceID].addComponent<rtc::Stat>(stat);
            ettMgr[spaceID].addComponent<rtc::Movement>();
            (*_lua)["playerTable"]["init"](spaceID, stat.hp, stat.attack, stat.defense, stat.speed, stat.shield);
            return spaceID;
        }

        template <typename Map, typename ...Args>
        static Entity::ID createPlayerSpaceShip(Map transitionMap, Args &&...args) noexcept
        {
            Entity::ID spaceID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            ettMgr[spaceID].addComponent<rtc::Player>(transitionMap);
            ettMgr[spaceID].addComponent<rtc::GameFieldLayer>();
            ettMgr[spaceID].addComponent<rtc::Lua>("player.lua", "player", "playerTable");
            ettMgr[spaceID].addComponent<rtc::Allied>();
            return createSpaceShip(spaceID, std::forward<Args>(args)...);
        }

        static Entity::ID createBullet(Entity::ID shooter, sf::Texture &texture,
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
            ettMgr[bulletID].addComponent<rtc::Lua>("standardAttackBullet.lua", "bullet", "standardAttackBulletTable");
            ettMgr[bulletID].addComponent<rtc::Allied>();
            ettMgr[bulletID].addComponent<rtc::Bullet>();
            ettMgr[bulletID].addComponent<rtc::SoundEffect>(eff);
            ettMgr[bulletID].addComponent<rtc::GameFieldLayer>();
            lua::LuaManager &lua = *_lua;
            lua["standardAttackBulletTable"]["init"](bulletID, shooter);
            return bulletID;
        }

        template<typename T>
        static Entity::ID createFieldItem(sf::Texture &texture,
                                          const sf::Vector2f &pos,
                                          float speed,
                                          float scale,
                                          [[maybe_unused]] char alpha,
                                          rtc::LayerType layer,
                                          T *subField,
                                          unsigned int ItemPackIndex) noexcept
        {
            Entity::ID itemID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            ettMgr[itemID].addComponent<rtc::FieldItem<T>>(subField, ItemPackIndex);
            sf::Sprite &sprite = ettMgr[itemID].addComponent<rtc::Sprite>(texture).sprite;
            sprite.setPosition(pos);
            sprite.scale(scale, scale);
            ettMgr[itemID].addComponent<rtc::Speed>(speed);
            switch (layer) {
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

        static Entity::ID createEnemy(sf::Texture &texture,
                                      float scale,
                                      float ypos,
                                      std::string &script,
                                      unsigned int nbArgs,
                                      std::vector<float> args,
                                      std::string &name,
                                      rtc::Stat &stat)
        {
            Entity ::ID ID = _ettMgr->createEntity();
            auto &ettMgr = *_ettMgr;
            sf::Vector2f pos(1921.0, ypos);
            ettMgr[ID].addComponent<rtc::Enemy>();
            ettMgr[ID].addComponent<rtc::GameFieldLayer>();
            sf::Sprite &sprite = ettMgr[ID].addComponent<rtc::Sprite>(texture).sprite;
            sprite.setPosition(pos);
            sprite.setScale(scale, scale);
            ettMgr[ID].addComponent<rtc::BoundingBox>(pos, sf::Vector2f(sprite.getTextureRect().width * scale,
                                                                        sprite.getTextureRect().height * scale));
            ettMgr[ID].addComponent<rtc::Stat>(stat);
            ettMgr[ID].addComponent<rtc::Lua>(script + ".lua", name, script + "Table");
            _quadtree->insert(ID);


            lua::LuaManager &lua = *_lua;
            switch(nbArgs) {
                case 0 :
                    lua[script + "Table"]["init"]();
                    break;
                case 1 :
                    lua[script + "Table"]["init"](ID, args[0]);
                    break;
                case 2 :
                    lua[script + "Table"]["init"](ID, args[0], args[1]);
                    break;
                case 3 :
                    lua[script + "Table"]["init"](ID, args[0], args[1], args[2]);
                    break;
                case 4 :
                    lua[script + "Table"]["init"](ID, args[0], args[1], args[2], args[3]);
                    break;
                case 5 :
                    lua[script + "Table"]["init"](ID, args[0], args[1], args[2], args[3], args[4]);
                    break;
                case 6 :
                    lua[script + "Table"]["init"](ID, args[0], args[1], args[2], args[3], args[4], args[5]);
                    break;
            }

            return ID;
        }

    private:
        static EntityManager *_ettMgr;
        static QuadTree<EntityManager> *_quadtree;
        static lua::LuaManager *_lua;
    };
}

#endif //RTYPE_GAMEFACTORY_HPP
