//
// Created by milerius on 20/01/18.
//

#ifndef RTYPE_SCENARIO_ACTION_HPP
#define RTYPE_SCENARIO_ACTION_HPP

#include <memory>
#include <queue>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/utils/Timer.hpp>
#include <log/Logger.hpp>
#include <rtype/entity/GameFactory.hpp>
#include <rtype/utils/DemoUtils.hpp>

namespace scenario
{
    struct ActionType
    {
        virtual std::string name() noexcept = 0;
        virtual void run() noexcept = 0;
    };

    class Action
    {
    public:

        explicit Action(rtype::EntityManager &ettMgr) noexcept : _ettMgr(ettMgr)
        {
        }

        template <typename ActionType, typename ...Args>
        void add(Args &&...args)
        {
            _actions.push(std::make_shared<ActionType>(std::forward<Args>(args)...));
        }

        template <typename ActionType>
        std::shared_ptr<ActionType> get() noexcept
        {
            if (_actions.empty())
                return nullptr;
            const auto ptr = _actions.front();
            _actions.pop();
            return std::static_pointer_cast<ActionType>(ptr);
        }

        void execute() noexcept
        {
            const auto ptr = _actions.front();
            _log(logging::Debug) << "execute : " << ptr->name() << std::endl;
            _actions.pop();
            ptr->run();
        }

        bool operator!() noexcept
        {
            return _actions.empty();
        }

        bool isWaiting() noexcept
        {
            return _isWaiting;
        }

        void setWait(bool wait, unsigned int millisecond = 0, bool killAll = false) noexcept
        {
            _isWaiting = wait;
            if (wait) {
                _toWait = std::chrono::milliseconds(millisecond);
                _killAll = killAll;
                _stopWatch.start();
            } else {
                _stopWatch.stop();
            }
        }

        bool isWaitDone() noexcept
        {
            if (!_killAll)
                return _stopWatch.elapsedMilli() >= _toWait;
            unsigned int nbEnemyEtt = 0;
            _ettMgr.for_each<rtype::components::Enemy>([&nbEnemyEtt]([[maybe_unused]]rtype::Entity &ett){
               ++nbEnemyEtt;
            });
            return nbEnemyEtt == 0;
        }

    public:
        using BasePtr = std::shared_ptr<ActionType>;

    private:
        std::queue<BasePtr> _actions;
        rtype::EntityManager &_ettMgr;
        bool _isWaiting{false};
        std::chrono::milliseconds _toWait{0};
        bool _killAll{false};
        rtype::utils::StopWatch _stopWatch;
        logging::Logger _log{"ScenarioAction", logging::Debug};

    };

    struct Music final : ActionType
    {
        Music(rtype::Configuration::Music _ID, gutils::EventManager &evtMgr) noexcept
                : ID(_ID), _evtMgr(evtMgr)
        {
        }

        std::string name() noexcept final
        {
            return "music";
        }

        void run() noexcept final
        {
            _evtMgr.emit<gutils::evt::PlayMusic>(ID, true);
        }

        rtype::Configuration::Music ID;
        gutils::EventManager &_evtMgr;
    };

    struct StopMusic final : ActionType
    {
        StopMusic(rtype::Configuration::Music _ID, gutils::EventManager &evtMgr) noexcept
            : ID(_ID), _evtMgr(evtMgr)
        {

        }

        std::string name() noexcept final
        {
            return "stopMusic";
        }

        void run() noexcept final
        {
            _evtMgr.emit<gutils::evt::StopMusic>(ID);
        }

        rtype::Configuration::Music ID;
        gutils::EventManager &_evtMgr;
    };

    struct Wait final : ActionType
    {
        explicit Wait(unsigned int _duration, Action &action_) noexcept
                : duration(_duration), action(action_)
        {
        }

        std::string name() noexcept final
        {
            return "wait";
        }

        void run() noexcept final
        {
            action.setWait(true, duration);
        }

        unsigned int duration;
        Action &action;
    };

    struct Spawn final : ActionType
    {
        explicit Spawn(std::string &&_ID, std::string &&texture_,
                       float scale_,
                       float startY_,
                       rtype::components::Stat &&stat_,
                       std::string &&script_,
                       unsigned int nbArg_,
                       std::vector<float> &&arg_,
                       sfutils::ResourceManager<sf::Texture, rtype::demo::SpriteT> &textures_) noexcept
                : ID(std::move(_ID)),
                  texture(texture_),
                  script(script_),
                  scale(scale_),
                  startY(startY_),
                  stat(stat_),
                  nbArgs(nbArg_),
                  args(arg_),
                  textures(textures_)
        {
        }

        std::string name() noexcept final
        {
            return "spawn";
        }

        void run() noexcept final
        {
            rtype::demo::SpriteT sprite = rtype::demo::Sprite(texture);
            rtype::GameFactory::createEnemy(textures.get(sprite), scale, startY, script, nbArgs, args, ID, stat);
        }

        std::string ID;
        std::string texture;
        std::string script;
        float scale;
        float startY;
        rtype::components::Stat stat;
        unsigned int nbArgs;
        std::vector<float> args;
        sfutils::ResourceManager<sf::Texture, rtype::demo::SpriteT> &textures;
    };

    struct WaitKillAll final : ActionType
    {
        explicit WaitKillAll(Action &action_) noexcept
                : action(action_)
        {
        }

        std::string name() noexcept final
        {
            return "waitKillAll";
        }

        void run() noexcept final
        {
            action.setWait(true, 0, true);
        }

        Action &action;
    };
}

#endif //RTYPE_SCENARIO_ACTION_HPP
