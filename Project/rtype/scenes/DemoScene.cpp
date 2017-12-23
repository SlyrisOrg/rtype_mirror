//
// Created by milerius on 22/12/17.
//

#include <rtype/scenes/DemoScene.hpp>

namespace rtype
{
    void DemoScene::enter() noexcept
    {
        showEnterScene();
    }

    void DemoScene::resume() noexcept
    {
        showResumeScene();
    }

    void DemoScene::pause() noexcept
    {
        showPauseScene();
    }

    void DemoScene::leave() noexcept
    {
        showLeavingScene();
    }

    void DemoScene::draw() noexcept
    {
    }

    void DemoScene::update([[maybe_unused]] double timeSinceLastFrame) noexcept
    {
    }

    bool DemoScene::keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept
    {
        return false;
    }

    bool DemoScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool DemoScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool DemoScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool DemoScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }
}