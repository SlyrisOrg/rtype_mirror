//
// Created by roman sztergbaum on 16/09/2017.
//

#ifndef RTYPE_WORLD_HPP
#define RTYPE_WORLD_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <utils/NonCopyable.hpp>
#include <rtype/config/Configuration.hpp>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/gutils/manager/SystemManager.hpp>

namespace rtype
{
    class World : private utils::NonCopyable
    {
    public:
        World()
        {
            _win.setMouseCursorVisible(false);
            _win.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width / 2) - cfg::game::width / 2,
                                          (sf::VideoMode::getDesktopMode().height / 2) - cfg::game::height / 2));
        }

    protected:
        sf::RenderWindow _win{sf::VideoMode(cfg::game::width, cfg::game::height), cfg::title, sf::Style::None};
        gutils::EventManager _evtMgr;
        gutils::SystemManager _systemMgr{_evtMgr};
    };
}

#endif //RTYPE_WORLD_HPP
