//
// Created by milerius on 24/01/18.
//

#include <rtype/scenes/GameScene.hpp>

namespace rtype
{
    void GameScene::enter() noexcept
    {
        showEnterScene();
    }

    void GameScene::resume() noexcept
    {
        showResumeScene();
    }

    void GameScene::pause() noexcept
    {
        showPauseScene();
    }

    void GameScene::draw() noexcept
    {
    }

    void GameScene::leave() noexcept
    {
        showLeavingScene();
    }

    void GameScene::update([[maybe_unused]] double timeSinceLastFrame) noexcept
    {
    }

    bool GameScene::keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept
    {
        //! Temporary
        if (evt.key == sf::Keyboard::Escape) {
            _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
        }
        return false;
    }

    bool GameScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool GameScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool GameScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool GameScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void GameScene::receive([[maybe_unused]] const gutils::evt::GameHostPort &hostPort) noexcept
    {
        _remoteGamePort = hostPort.hostGamePort;
        _log(lg::Info) << "Using " << _remoteGamePort << " as remote game port." << std::endl;
    }
}