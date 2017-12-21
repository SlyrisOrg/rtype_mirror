//
// Created by zouz on 21/12/17.
//

#include <rtype/gui/GUIManager.hpp>
#include <rtype/scenes/private/ForwardEnumLobby.hpp>
#include <rtype/scenes/LobbyScene.hpp>

namespace rtype
{
    //! Public Member Function
    void LobbyScene::draw() noexcept
    {
        _win.draw(_bg);
    }

    void LobbyScene::enter() noexcept
    {
        showEnterScene();
        __configure();
    }

    void LobbyScene::resume() noexcept
    {
    }

    void LobbyScene::pause() noexcept
    {
    }

    void LobbyScene::leave() noexcept
    {
    }

    void LobbyScene::update([[maybe_unused]] double timeSinceLastFrame) noexcept
    {
    }

    bool LobbyScene::keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept
    {
        if (evt.key == sf::Keyboard::Escape) {
            __goBacktoQueue();
        }
        return false;
    }

    bool LobbyScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool LobbyScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool LobbyScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool LobbyScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    //! Private member function
    void LobbyScene::__configure() noexcept
    {
        auto start = __setGUI();
        if (start) {
            __subscribeEvents();
            __initTexture();
            __resetUI();
        }
    }

    bool LobbyScene::__setGUI() noexcept
    {
        using namespace cfg::lobby;
        return GUIManager::setGUI<LobbyWidgets, nbWidgets>(lobbyLayout, _gui, _log);
    }

    void LobbyScene::__initTexture() noexcept
    {
        _bg.setTexture(Configuration::textures.get(Configuration::Sprite::QueueBackground));
        float x = _gui[LobbyWidgets::BorderFour].getPosition().d_x.d_offset + 6.0f;
        float y = _gui[LobbyWidgets::BorderFour].getPosition().d_y.d_offset + 12.0f;
        float offset = _gui[LobbyWidgets::BorderTwo].getPosition().d_x.d_offset -
                       _gui[LobbyWidgets::BorderFour].getPosition().d_x.d_offset;
        sf::Vector2f pos(x, y);
        for (unsigned int i = 0; i < 4; ++i) {
            auto &spr = _bordersBg.emplace_back();
            spr.setTexture(Configuration::textures.get(Configuration::Sprite::BorderBG));
            spr.setPosition(pos);
            pos.x += offset;
        }
    }

    void LobbyScene::__subscribeEvents() noexcept
    {
    }

    void LobbyScene::__goBacktoQueue() noexcept
    {
        _evtMgr.emit<gutils::evt::ChangeScene>(rtype::Scene::Queue);
    }

    void LobbyScene::__resetUI() noexcept
    {
        _gui[LobbyWidgets::Timer].setPosition(_gui[LobbyWidgets::Timer].getPosition());
        _gui[LobbyWidgets::DiscardButton].hide();
        _gui[LobbyWidgets::PlayerOneButton].setProperty("Image", "RTButtonBar/tab_a");
        _gui[LobbyWidgets::PlayerTwoButton].setProperty("Image", "RTButtonBar/tab_a");
        _gui[LobbyWidgets::PlayerThreeButton].setProperty("Image", "RTButtonBar/tab_a");
        _gui[LobbyWidgets::PlayerFourButton].setProperty("Image", "RTButtonBar/tab_a");
    }
}
