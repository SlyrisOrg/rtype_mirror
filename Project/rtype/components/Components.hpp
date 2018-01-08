//
// Created by milerius on 08/01/18.
//

#ifndef RTYPE_COMPONENTS_HPP
#define RTYPE_COMPONENTS_HPP

#include <SFML/Graphics/Sprite.hpp>

namespace rtype::components
{
    struct Sprite
    {
        template <typename ...Args>
        Sprite(Args &&...args) : sprite(std::forward<Args>(args)...)
        {
        }

        sf::Sprite sprite;
    };

    struct BoundingBox
    {
        // Construct BoundinxBox with Sprite, Rectangle, Circle
        template <typename SFMLObject>
        BoundingBox(const SFMLObject obj) : AABB(obj.getGlobalBounds())
        {
        }

        //! Cover x,y,width,height / sf::vector2f Position/Size
        template <typename ...Args>
        BoundingBox(Args &&...args) : AABB(std::forward<Args>(args)...)
        {
        }

        sf::FloatRect AABB;
    };
}

#endif //RTYPE_COMPONENTS_HPP
