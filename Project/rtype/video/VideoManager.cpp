//
// Created by milerius on 17/11/17.
//

#include <rtype/video/VideoManager.hpp>

//! Constructors
namespace rtype
{
    VideoManager::VideoManager(gutils::EventManager &evtMgr, sf::RenderWindow &win) noexcept :
        _evtMgr(evtMgr),
        _window(win)
    {
        _evtMgr.subscribe<gutils::evt::LaunchVideo>(*this);
        _evtMgr.subscribe<gutils::evt::StopVideo>(*this);
    }
}

//! Public member functions
namespace rtype
{
    void VideoManager::draw() noexcept
    {
        if (_active) {
            _window.draw(*_movie);
            _window.draw(_fade);
        }
    }
}

//! Callbacks
namespace rtype
{
    void VideoManager::receive(const gutils::evt::LaunchVideo &evt) noexcept
    {
        _movie = evt.ptr;
        setMovie(evt.videoName, evt.scene);
        _movie->setVolume(cfg::game::musicVolume);
    }

    void VideoManager::receive([[maybe_unused]] const gutils::evt::StopVideo &evt) noexcept
    {
        _movie->stop();
        __end();
    }
}

namespace rtype
{
    void VideoManager::__end() noexcept
    {
        _log(logging::Info) << "End of video : " << _movieName << ".avi" << std::endl;
        _active = false;
        _window.setVerticalSyncEnabled(false);
        if (_sceneToGoAfterVideo != Scene::NoScene)
            _evtMgr.emit<gutils::evt::ChangeScene>(_sceneToGoAfterVideo);
    }

    void VideoManager::__start() noexcept
    {
        _window.setVerticalSyncEnabled(true);
        _fade.setFillColor(sf::Color::Transparent);
        _active = true;
        _movie->fit(0.f, 0.f, static_cast<float>(cfg::game::width), static_cast<float>(cfg::game::height));
        sf::Time time;
        _movie->setPlayingOffset(time);
        _movie->play();
    }

    void VideoManager::setMovie(const std::string &movieName, Scene sceneToGoAfterVideo) noexcept
    {
        if (sceneToGoAfterVideo != Scene::NoScene)
            _sceneToGoAfterVideo = sceneToGoAfterVideo;
        _movieName = movieName;
        _log(logging::Info) << "Starting video : " << _movieName << ".avi" << std::endl;
        __start();
    }

    void VideoManager::update() noexcept
    {
        if (_active) {
            auto duration = _movie->getDuration().asSeconds();
            auto now = _movie->getPlayingOffset().asSeconds();
            if (duration - now < 3.2f && _fade.getFillColor().a < 255) {
                _fade.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(_fade.getFillColor().a + 1)));
            }

            if (_movie->getStatus() == sfe::Status::Stopped) {
                __end();
            }
            _movie->update();
        }
    }
}