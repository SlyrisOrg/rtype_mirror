//
// Created by roman sztergbaum on 16/09/2017.
//

#ifndef RTYPE_SEVENTS_HPP
#define RTYPE_SEVENTS_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <rtype/gutils/base/BaseSystem.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>

namespace rtype
{
    class SEvents : public gutils::System<SEvents>, public gutils::Receiver<SEvents>
    {
    public:
        explicit SEvents(gutils::EventManager &mgr, sf::RenderWindow &win) noexcept :
            gutils::System<SEvents>(mgr, "Events"), _win(win)
        {
            _evtMgr.subscribe<evt::FullScreen>(*this);
        }

    private:
        void __handleFullScreen() noexcept
        {
            if (!cfg::game::fullscreen) {
                sf::VideoMode mode(cfg::game::width, cfg::game::height);
                _win.create(mode, cfg::title, sf::Style::Fullscreen);
                _win.setMouseCursorVisible(false);
                cfg::game::fullscreen = !cfg::game::fullscreen;
            } else {
                sf::VideoMode mode(cfg::game::width, cfg::game::height);
                _win.create(mode, cfg::title, sf::Style::None);
                _evtMgr.emit<evt::RestoreDefault>(_win);
                cfg::game::fullscreen = !cfg::game::fullscreen;
            }
        }

    public:
        void receive([[maybe_unused]] const evt::FullScreen& evt) noexcept
        {
            __handleFullScreen();
        }

        void update([[maybe_unused]] double deltaTime) noexcept override
        {
            namespace evt = gutils::evt;
            sf::Event event;

            while (_win.pollEvent(event)) {
                _evtMgr.emit<gutils::evt::GuiEvents>(event);
                auto code = event.key.code;
                auto key = event.key;
                switch (event.type) {
                    case sf::Event::Closed:
                        _evtMgr.emit<gutils::evt::GameShutdown>();
                        break;
                    case sf::Event::Resized:
                        _evtMgr.emit<gutils::evt::WindowResized>(event.size.width, event.size.height);
                        break;
                    case sf::Event::LostFocus:
                        break;
                    case sf::Event::GainedFocus:
                        break;
                    case sf::Event::TextEntered:
                        break;
                    case sf::Event::KeyPressed:
                        (code == sf::Keyboard::F11) ? __handleFullScreen() : _evtMgr.emit<evt::KeyPressed>(key);
                        break;
                    case sf::Event::KeyReleased:
                        _evtMgr.emit<evt::KeyReleased>(key);
                        break;
                    case sf::Event::MouseWheelMoved:
                        break;
                    case sf::Event::MouseWheelScrolled:
                        break;
                    case sf::Event::MouseButtonPressed:
                        break;
                    case sf::Event::MouseButtonReleased:
                        break;
                    case sf::Event::MouseMoved:
                        break;
                    case sf::Event::MouseEntered:
                        break;
                    case sf::Event::MouseLeft:
                        break;
                    case sf::Event::JoystickButtonPressed:
                        break;
                    case sf::Event::JoystickButtonReleased:
                        break;
                    case sf::Event::JoystickMoved:
                        break;
                    case sf::Event::JoystickConnected:
                        break;
                    case sf::Event::JoystickDisconnected:
                        break;
                    case sf::Event::TouchBegan:
                        break;
                    case sf::Event::TouchMoved:
                        break;
                    case sf::Event::TouchEnded:
                        break;
                    case sf::Event::SensorChanged:
                        break;
                    case sf::Event::Count:
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        sf::RenderWindow &_win;
    };
}

#endif //RTYPE_SEVENTS_HPP
