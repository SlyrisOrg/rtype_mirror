//
// Created by doom on 23/12/17.
//

#ifndef RTYPE_COMPONENTS_HPP
#define RTYPE_COMPONENTS_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <meta/List.hpp>

namespace game::comps
{
    struct Box
    {
        Box(unsigned int x, unsigned int y, unsigned int width, unsigned int height) noexcept :
            x(x), y(y), width(width), height(height)
        {
        }

        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
    };

    struct Collidable
    {
    };

    struct Player
    {
    };

    struct Enemy
    {
    };

    using Components = meta::TypeList<Box, Collidable, Player, Enemy>;
}

#endif //RTYPE_COMPONENTS_HPP
