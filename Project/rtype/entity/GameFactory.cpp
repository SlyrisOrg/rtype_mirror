//
// Created by milerius on 09/01/18.
//

#include <rtype/entity/GameFactory.hpp>

namespace rtype
{
    EntityManager *GameFactory::_ettMgr = nullptr;
    QuadTree<EntityManager> *GameFactory::_quadtree = nullptr;
    lua::LuaManager *GameFactory::_lua = nullptr;
}