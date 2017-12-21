//
// Created by zouz on 21/12/17.
//

#include <rtype/gui/GUIManager.hpp>
#include <rtype/scenes/private/ForwardEnumQueue.hpp>
#include "QueueScene.hpp"

namespace rtype
{
    void QueueScene::enter() noexcept
    {
        showEnterScene();
        __configure();
    }

    void QueueScene::resume() noexcept
    {
        showResumeScene();
    }

    void QueueScene::pause() noexcept
    {
        showPauseScene();
    }

    void QueueScene::draw() noexcept
    {
        _win.draw(_bg);
    }

    void QueueScene::leave() noexcept
    {
        if (_gui.sheet != nullptr)
            GUIManager::destroySheet(*_gui.sheet);
        showLeavingScene();
        __queueMusicStop();
    }

    void QueueScene::update([[maybe_unused]]double timeSinceLastFrame) noexcept
    {
    }

    bool QueueScene::keyPressed([[maybe_unused]]const gutils::evt::KeyPressed &evt) noexcept
    {
        return false;
    }

    bool QueueScene::keyReleased([[maybe_unused]]const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool QueueScene::mouseMoved([[maybe_unused]]const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool QueueScene::mousePressed([[maybe_unused]]const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool QueueScene::mouseReleased([[maybe_unused]]const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void QueueScene::__configure() noexcept
    {
        auto start = __setGUI();
        if (start) {
            __queueMusicStart();
            __subscribeEvents();
            __resetUi();
            __initTextures();
        }
    }

    bool QueueScene::__setGUI() noexcept
    {
        using namespace cfg::queue;
       return GUIManager::setGUI<QueueWidgets, nbWidgets>(queueLayout, _gui, _log);
    }

    void QueueScene::__subscribeEvents() noexcept
    {
        using namespace cfg::queue;
        using pb = CEGUI::PushButton;
        using evt = CEGUI::Event;
        using Qs = QueueScene;

        _gui[QueueWidgets::DiscardButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&Qs::__goBackToProfil, this));
    }

    void QueueScene::__goBackToProfil() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
    }

    void QueueScene::__resetUi() noexcept
    {
        _gui[QueueWidgets::Timer].setPosition(_gui[QueueWidgets::Timer].getPosition());
    }

    void QueueScene::__queueMusicStart() noexcept
    {
        _evtMgr.emit<gutils::evt::PlayMusic>(Configuration::Music::DarkVoidAmb);
    }

    void QueueScene::__queueMusicStop() noexcept
    {
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::DarkVoidAmb);
    }

    void QueueScene::__initTextures() noexcept
    {
        _bg.setTexture(Configuration::textures.get(Configuration::Sprite::QueueBackground));
    }
}

