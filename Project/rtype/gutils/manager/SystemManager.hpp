//
// Created by roman sztergbaum on 16/09/2017.
//

#ifndef GUTILS_SYSTEMMANAGER_HPP
#define GUTILS_SYSTEMMANAGER_HPP

#include <cassert>
#include <vector>
#include <memory>
#include <utils/NonCopyable.hpp>
#include <rtype/gutils/base/BaseSystem.hpp>

namespace gutils
{
    class EventManager;
}

namespace gutils
{
    class SystemManager : utils::NonCopyable
    {
    public:
        explicit SystemManager(EventManager &evtMgr) noexcept : _eventMgr(evtMgr)
        {
        }

        template <typename S>
        void add(std::shared_ptr<S> system)
        {
            _system.emplace_back(system);
        }

        template <typename S, typename ... Args>
        std::shared_ptr<S> add(Args &&... args)
        {
            std::shared_ptr<S> s = std::make_shared<S>(_eventMgr, std::forward<Args>(args)...);
            add(s);
            return s;
        }

        //! The idx is here for skip some system if you want.
        void update(double deltaTime, unsigned int idx = 0) const
        {
            for (auto cur = _system.begin() + idx; cur != _system.end(); cur++) {
                (*cur)->update(deltaTime);
            }
        }

        BaseSystem &operator[](unsigned int idx)
        {
            return *_system[idx];
        }

    private:
        EventManager &_eventMgr;
        using SystemPtr = std::shared_ptr<BaseSystem>;
        std::vector<SystemPtr> _system;
    };
}

#endif //GUTILS_SYSTEMMANAGER_HPP
