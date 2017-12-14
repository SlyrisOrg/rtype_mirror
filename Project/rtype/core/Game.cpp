//
// Created by roman sztergbaum on 16/09/2017.
//

#include <SFML/Graphics.hpp>
#include <rtype/core/Game.hpp>
#include <rtype/systems/Systems.hpp>

namespace lg = logging;

namespace rtype
{
    Game::Game() noexcept
    {
        _evtMgr.subscribe<evt::GameShutdown>(*this);
        _evtMgr.subscribe<evt::RestoreDefault>(*this);
    }

    Game::~Game() noexcept
    {
        _log(lg::Info) << "Shutting down RType." << std::endl;
    }

    //! Private member functions
    void Game::__initSystems()
    {
        _systemMgr.add<SEvents>(_win);
        _systemMgr.add<SGui>(_win);
        _systemMgr.add<SScenes>(_win);
        _systemMgr.add<SVideo>(_win);
        _systemMgr.add<SAudio>();
        _log(lg::Info) << "Successfully initialized RType" << std::endl;
        _evtMgr.emit<evt::EnterScene>(Scene::Loading);
    }

    void Game::__sysUpdate(const sf::Time &time) noexcept
    {
        _systemMgr[Sys::Gui].update(time.asSeconds());
        _systemMgr[Sys::Audio].update(time.asSeconds());
        _systemMgr[Sys::Video].update(time.asSeconds());
        _systemMgr[Sys::Scene].update(time.asSeconds());
    }

    void Game::__sysDraw() noexcept
    {
        _systemMgr[Sys::Video].draw();
        _systemMgr[Sys::Scene].draw();
        _systemMgr[Sys::Gui].draw();
    }

    void Game::__render()
    {
        _win.clear();
        __sysDraw();
        _win.display();
    }

    void Game::__update(const sf::Time &time) noexcept
    {
        __sysUpdate(time);
    }

    void Game::__launch() noexcept
    {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate;
        sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

        while (!_isRunning) {
            _systemMgr[Sys::Event].update(TimePerFrame.asSeconds());
            timeSinceLastUpdate = clock.restart();
            while (timeSinceLastUpdate > TimePerFrame) {
                timeSinceLastUpdate -= TimePerFrame;
                __update(TimePerFrame);
            }
            __update(timeSinceLastUpdate);
            __render();
        }
    }

    //! Public member functions
    void Game::receive([[maybe_unused]] const evt::GameShutdown &evt) noexcept
    {
        _isRunning = true;
    }


    void Game::receive(const evt::RestoreDefault &evt) noexcept
    {
        World::initialState(evt.win);
    }

    void Game::run()
    {
        __initSystems();
        __launch();
    }
}