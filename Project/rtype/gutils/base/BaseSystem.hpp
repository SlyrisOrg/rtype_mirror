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
        BaseSystem(gutils::EventManager &evtMgr) : _evtMgr(evtMgr)
        {
        }

    public:
        ~BaseSystem() noexcept override = default;
    public:
        //! update the current system
        virtual void update(double deltaTime) = 0;

        //! Window / Gui / Scene System can draw
        virtual void draw()
        {};
    protected:
        gutils::EventManager &_evtMgr;
    };

    template <typename Derived>
    class System : public BaseSystem
    {
    public:
        System(gutils::EventManager &evtMgr, std::string_view systemName) noexcept : BaseSystem(evtMgr),
                                                                                     _systemName(systemName)
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
        logging::Logger _log{_systemName, logging::Info};
    };
}

#endif //GUTILS_ISYSTEM_HPP
