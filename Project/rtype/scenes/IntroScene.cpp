//
// Created by milerius on 10/11/17.
//

#include <rtype/core/SafeCEGUI.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/scenes/IntroScene.hpp>

//! Public member functions
namespace rtype
{
    void IntroScene::enter() noexcept
    {
        showEnterScene();
        _evtMgr.emit<gutils::evt::LaunchVideo>(Configuration::Video::Intro, Scene::Login);
        CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
        _win.resetGLStates();
    }

    void IntroScene::resume() noexcept
    {
    }

    void IntroScene::pause() noexcept
    {
    }

    void IntroScene::leave() noexcept
    {
        showLeavingScene();
    }

    void IntroScene::update([[maybe_unused]] double timeSinceLastFrame) noexcept
    {
    }

    bool IntroScene::keyPressed(const gutils::evt::KeyPressed &evt) noexcept
    {
        if (evt.key == sf::Keyboard::Escape) {
            _evtMgr.emit<gutils::evt::StopVideo>();
        }
        return true;
    }

    bool IntroScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool IntroScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool IntroScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool IntroScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void IntroScene::draw() noexcept
    {
    }
}