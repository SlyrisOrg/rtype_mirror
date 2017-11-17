//
// Created by roman sztergbaum on 16/09/2017.
//

#ifndef RTYPE_GAME_HPP
#define RTYPE_GAME_HPP

#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/core/World.hpp>

namespace evt = gutils::evt;

namespace rtype
{
    class Game : public World, public gutils::Receiver<Game>
    {
    public:
        Game() noexcept;
        ~Game() noexcept override;

    public:
        void receive(const evt::GameShutdown &evt) noexcept;
        void run();

    private:
        void __initSystems();
        void __launch() noexcept;
        void __update(const sf::Time &time) noexcept;
        void __sysUpdate(const sf::Time &time) noexcept;
        void __sysDraw() noexcept;

    private:
        bool _shutDown{false};
        logging::Logger _log{"RType", logging::Info};
    };
}

#endif //RTYPE_GAME_HPP
