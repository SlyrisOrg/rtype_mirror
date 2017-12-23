//
// Created by szterg_r on 23/10/2017.
//

#ifndef RTYPE_SVIDEO_HPP
#define RTYPE_SVIDEO_HPP

#include <rtype/video/VideoManager.hpp>
#include <rtype/gutils/base/BaseSystem.hpp>

namespace rtype
{
    class SVideo : public gutils::System<SVideo>
    {
    public:
        explicit SVideo(gutils::EventManager &mgr, sf::RenderWindow &win) noexcept :
            gutils::System<SVideo>(mgr, "Video"),
            _videoMgr(mgr, win)
        {
        }

        void update([[maybe_unused]] double deltaTime) noexcept override
        {
            _videoMgr.update();
        }

        void draw() noexcept
        {
            _videoMgr.draw();
        }

    private:
        VideoManager _videoMgr;
    };
}

#endif //RTYPE_SVIDEO_HPP
