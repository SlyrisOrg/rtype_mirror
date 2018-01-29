//
// Created by king on 1/26/18.
//

#ifndef RTYPE_COLLISIONSYSTEM_HPP
#define RTYPE_COLLISIONSYSTEM_HPP

#include <deque>
#include <utils/Enums.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/lua/LuaManager.hpp>
#include <rtype/utils/QuadTree.hpp>
#include <log/Logger.hpp>

namespace rtype
{
    class CollisionSystem
    {
    public:
        explicit CollisionSystem(EntityManager &ettMgr, QuadTree<EntityManager > &quadtree, lua::LuaManager &lua) noexcept
                : _ettMgr(ettMgr), _lua(lua), _quadtree(quadtree)
        {
        }

        void update([[maybe_unused]]double timeSinceLastFrame) noexcept
        {
            _ettMgr.for_each<rtc::BoundingBox>([this](Entity &ett){
                QuadTree<EntityManager>::QuadNode *node = _quadtree.getNode(ett.getID());
                if (node != nullptr
                    && std::find(_nodes.begin(), _nodes.end(), node) == _nodes.end()
                    && node->_items.size() >= 2) {
                    _nodes.push_back(node);
                }
            });

            std::deque<size_t> itemsQ;
            for (auto &&node : _nodes) {
                for (auto &&a : node->_items) {
                    itemsQ.push_back(a);
                }
                size_t size = itemsQ.size();
                for (size_t i = 0; i < size - 1; ++i){
                    Entity::ID curItem = itemsQ.front();
                    itemsQ.pop_front();
                    for (auto &&item : itemsQ) {;
                        if (_ettMgr[curItem].getComponent<rtc::BoundingBox>().AABB.intersects(
                                _ettMgr[item].getComponent<rtc::BoundingBox>().AABB)) {
                            _lua[_ettMgr[curItem].getComponent<rtc::Lua>().tableName]["onCollide"](curItem, item);
                            _lua[_ettMgr[item].getComponent<rtc::Lua>().tableName]["onCollide"](item, curItem);
                        }
                    }
                }
                itemsQ.clear();
            }

            _nodes.clear();
        }
    private:
        EntityManager &_ettMgr;
        lua::LuaManager &_lua;
        QuadTree<EntityManager> &_quadtree;
        std::vector<QuadTree<EntityManager>::QuadNode *> _nodes;
        logging::Logger _log{"collision", logging::Debug};
    };
}

#endif //RTYPE_COLLISIONSYSTEM_HPP
