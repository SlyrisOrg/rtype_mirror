//
// Created by milerius on 08/01/18.
//

#ifndef RTYPE_ECS_HPP
#define RTYPE_ECS_HPP

#include <rtype/components/Components.hpp>
#include <rtype/entity/EntityManager.hpp>

namespace rtype
{
    using Components = meta::TypeList<components::Sprite, components::BoundingBox>;
    using EntityManager = game::EntityManager<Components>;
    using Entity = EntityManager::Entity;
}

#endif //RTYPE_ECS_HPP
