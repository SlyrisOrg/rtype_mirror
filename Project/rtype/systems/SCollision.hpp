//
// Created by milerius on 08/01/18.
//

#ifndef RTYPE_SCOLLISION_HPP
#define RTYPE_SCOLLISION_HPP

#include <rtype/entity/ECS.hpp>
#include <rtype/gutils/base/BaseSystem.hpp>

namespace rtype
{
    /*
     * TODO: we will add more things when we will merge the QuadTree, you can find a complete exemple in
     * component-test.cpp.
     */
    class SCollision : public gutils::System<SCollision>
    {
    public:
        explicit SCollision(gutils::EventManager &evtMgr, EntityManager &ettMgr) noexcept :
            gutils::System<SCollision>(evtMgr, "Collision"),
            ettMgr(ettMgr)
        {
        }

        void update([[maybe_unused]] double deltaTime) noexcept override
        {
        }

        // TODO: We will add more complete functions later, pushed for the test at the moment work but it's not pixel perfect.
        bool boundingBoxCheck(const rtype::components::BoundingBox &object1,
                              const rtype::components::BoundingBox &object2)
        {
            return object1.AABB.intersects(object2.AABB);
        }

    private:
        EntityManager &ettMgr;
    };
}

#endif //RTYPE_SCOLLISION_HPP
