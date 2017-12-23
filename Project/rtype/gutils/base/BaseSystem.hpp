//
// Created by roman sztergbaum on 16/09/2017.
//

#ifndef GUTILS_ISYSTEM_HPP
#define GUTILS_ISYSTEM_HPP

#include <string_view>
#include <rtype/gutils/manager/EventManager.hpp>
#include <utils/NonCopyable.hpp>

namespace gutils
{
    class BaseSystem : private utils::NonCopyable
    {
    public:
        explicit BaseSystem(gutils::EventManager &evtMgr) noexcept : _evtMgr(evtMgr)
        {
        }

        virtual ~BaseSystem() noexcept = default;

    public:
        //! update the current system
        virtual void update(double deltaTime) noexcept = 0;

    protected:
        gutils::EventManager &_evtMgr;
    };

    template <typename Derived>
    class System : public BaseSystem
    {
    public:
        System(gutils::EventManager &evtMgr, std::string systemName) noexcept : BaseSystem(evtMgr),
                                                                                _systemName(std::move(systemName))
        {
            _log(logging::Debug) << "System " << _systemName << " successfully initialized." << std::endl;
        }

        ~System() noexcept override
        {
            _log(logging::Debug) << "System " << _systemName << " shutdown." << std::endl;
        }

    private:
        friend class SystemManager;

    protected:
        std::string _systemName;
        logging::Logger _log{_systemName, logging::Debug};
    };
}

#endif //GUTILS_ISYSTEM_HPP
