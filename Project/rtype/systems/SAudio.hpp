//
// Created by roman sztergbaum on 25/10/2017.
//

#ifndef RTYPE_SAUDIO_HPP
#define RTYPE_SAUDIO_HPP

#include <rtype/gutils/base/BaseSystem.hpp>
#include <rtype/audio/SoundManager.hpp>

namespace rtype
{
    class SAudio : public gutils::System<SAudio>
    {
    public:

        explicit SAudio(gutils::EventManager &evtMgr) noexcept : gutils::System<SAudio>(evtMgr, "Audio"), _soundMgr(evtMgr)
        {
        }

        void update([[maybe_unused]] double deltaTime) noexcept override
        {
            _soundMgr.update();
        }

    private:
        SoundManager _soundMgr;
    };
}

#endif //RTYPE_SAUDIO_HPP
