//
// Created by milerius on 10/01/18.
//

#include <rtype/config/PlayerConfig.hpp>

namespace cfg::player
{
    rtype::ActionMap<int> playerInputs;
    std::atomic<matchmaking::Mode> mode{matchmaking::Mode::Solo};
}