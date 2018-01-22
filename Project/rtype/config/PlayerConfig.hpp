//
// Created by milerius on 10/01/18.
//

#ifndef RTYPE_PLAYERCONFIG_HPP
#define RTYPE_PLAYERCONFIG_HPP

#include <atomic>
#include <rtype/protocol/MatchMaking.hpp>
#include <rtype/utils/Action.hpp>

namespace cfg::player
{
    enum PlayerInputs
    {
        Up,
        Left,
        Right,
        Down,
        SpaceShoot,
    };

    extern std::atomic<matchmaking::Mode> mode;

    extern rtype::ActionMap<int> playerInputs;

    static inline void setPlayerInputs() noexcept
    {
        playerInputs.add(PlayerInputs::Up, rtype::Action(sf::Keyboard::Up));
        playerInputs.add(PlayerInputs::Right, rtype::Action(sf::Keyboard::Right));
        playerInputs.add(PlayerInputs::Left, rtype::Action(sf::Keyboard::Left));
        playerInputs.add(PlayerInputs::Down, rtype::Action(sf::Keyboard::Down));
        playerInputs.add(PlayerInputs::SpaceShoot, rtype::Action(sf::Keyboard::Space));
    }
}

#endif //RTYPE_PLAYERCONFIG_HPP
