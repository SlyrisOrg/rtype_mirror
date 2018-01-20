//
// Created by milerius on 20/01/18.
//

#ifndef RTYPE_ACTION_HPP
#define RTYPE_ACTION_HPP

#include <memory>
#include <queue>

namespace scenario
{
    struct Type
    {
        virtual void run() noexcept = 0;
    };

    struct Music final : Type
    {
        Music(rtype::Configuration::Music _ID) noexcept : ID(_ID)
        {
        }

        void run() noexcept final
        {
            std::cout << "ID: " << ID << std::endl;
        }

        rtype::Configuration::Music ID;
    };

    struct Wait final : Type
    {
        explicit Wait(float _duration) noexcept : duration(_duration)
        {
        }

        void run() noexcept final
        {
            //TODO: Client Manager, emit event??
        }

        float duration;
    };

    struct Spawn final : Type
    {
        explicit Spawn(std::string &&_ID) noexcept : ID(std::move(_ID))
        {
        }

        void run() noexcept final
        {
            //TODO: emit event
        }

        std::string ID;
    };

    struct WaitKillAll final : Type
    {
        void run() noexcept final
        {
        }
    };

    class Action
    {
    public:
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
            _actions.pop();
            ptr->run();
        }

        bool operator!() noexcept
        {
            return _actions.empty();
        }

    public:
        using BasePtr = std::shared_ptr<Type>;

    private:
        std::queue<BasePtr> _actions;
    };
}

#endif //RTYPE_ACTION_HPP
