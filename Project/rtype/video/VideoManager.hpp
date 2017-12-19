//
// Created by szterg_r on 23/10/2017.
//

#ifndef RTYPE_VIDEOMANAGER_HPP
#define RTYPE_VIDEOMANAGER_HPP

#include <sfeMovie/Movie.hpp>
#include <rtype/gutils/manager/EventManager.hpp>

namespace rtype
{
    class VideoManager final : public gutils::Receiver<VideoManager>
    {
    public:
        explicit VideoManager(gutils::EventManager &evtMgr, sf::RenderWindow &win) noexcept;
        void receive(const gutils::evt::LaunchVideo &evt) noexcept;
        void receive(const gutils::evt::StopVideo &evt) noexcept;
        void draw() noexcept;
        void setMovie(const std::string &movieName, Scene sceneToGoAfterVideo = Scene::NoScene) noexcept;
        void update() noexcept;

    private:
        void __end() noexcept;
        void __start() noexcept;

    private:
        bool _active{false};
        std::string _movieName{""};
        sfe::Movie *_movie;
        Scene _sceneToGoAfterVideo;
        gutils::EventManager &_evtMgr;
        sf::RenderWindow &_window;
        logging::Logger _log{"VideoManager", logging::Info};
    };
}

#endif //RTYPE_VIDEOMANAGER_HPP
