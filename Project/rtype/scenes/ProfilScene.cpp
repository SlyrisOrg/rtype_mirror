//
// Created by milerius on 10/11/17.
//

#include <rtype/gui/GUIManager.hpp>
#include <rtype/config/ProfilConfig.hpp>
#include <rtype/scenes/ProfilScene.hpp>
#include <rtype/api/rtype-api.hpp>
#include <rtype/config/SpaceshipConfig.hpp>

namespace rtype
{
    void ProfilScene::__resume() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        resume();
    }

    void ProfilScene::__leaveGame() noexcept
    {
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::MarketInstrumentalAmb);
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        _evtMgr.emit<gutils::evt::GameShutdown>();
        leave();
    }

    void ProfilScene::__logOut() noexcept
    {
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::MarketInstrumentalAmb);
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Login);
        leave();
    }

    void ProfilScene::__optionsWindow() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        _settings.show();
        _gui[ProfilWidgets::MainMenu].setVisible(false);
        _gui[ProfilWidgets::MainMenu].disable();
    }

    void ProfilScene::__closeOptionsWindow() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        _settings.hide();
        _gui[ProfilWidgets::MainMenu].setVisible(true);
        _gui[ProfilWidgets::MainMenu].enable();
    }

    //! Private member functions
    void ProfilScene::__subscribeEvents() noexcept
    {
        using pb = CEGUI::PushButton;
        using evt = CEGUI::Event;
        using ps = ProfilScene;

        _gui[ProfilWidgets::BackButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&ps::__resume, this));
        _gui[ProfilWidgets::ExitDesktopButton].subscribeEvent(pb::EventClicked,
                                                              evt::Subscriber(&ps::__leaveGame, this));
        _gui[ProfilWidgets::LogOutButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&ps::__logOut, this));
        _gui[ProfilWidgets::OptionsButton].subscribeEvent(pb::EventClicked,
                                                          evt::Subscriber(&ps::__optionsWindow, this));
        _gui[ProfilWidgets::OptionsCloseButton].subscribeEvent(pb::EventClicked,
                                                               evt::Subscriber(&ps::__closeOptionsWindow, this));
        //TODO Temporary going to QScene, Need to offer game choice before.
        _gui[ProfilWidgets::PlayButton].subscribeEvent(pb::EventClicked,
                                                       evt::Subscriber(&ps::__goingToQueue, this));
    }

    bool ProfilScene::__setGUI() noexcept
    {
        using namespace cfg::profil;
        return GUIManager::setGUI<ProfilWidgets, cfg::profil::nbWidgets>(profilLayout, _gui, _log);
    }

    void ProfilScene::__setText() noexcept
    {
        using namespace cfg::profil;
        for (auto &&cur : profileLabel) {
            const auto &[widgetID, widgetTxt] = cur.second;
            _gui[widgetID].setText(widgetTxt);
        }
        _settings.resetValue();
    }

    void ProfilScene::__goingToQueue() noexcept
    {
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::MarketInstrumentalAmb);
        _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Queue);
    }

    void ProfilScene::__getData() const noexcept
    {
        std::error_code ec;
        auto res = API::requestGetData(cfg::game::tokenSession, ec).wait();
        if (res == pplx::task_group_status::completed) {
            if (!ec) {
                _log(lg::Info) << ec.message() << std::endl;
            } else
                _log(lg::Error) << ec.message() << std::endl;
        }
    }

    void ProfilScene::__setData() noexcept
    {
        using namespace cfg::profil;
        GUIManager::applyCEGUIFunc(&GUIManager::setText,
                                   std::make_pair(&_gui[ProfilWidgets::Pseudo], nickname),
                                   std::make_pair(&_gui[ProfilWidgets::GoldNormalMoney], gold));
        static_cast<CEGUI::ProgressBar &>(_gui[ProfilWidgets::XPBar]).setProgress(xp);
        _gui[cfg::profil::faction].setVisible(true);
        _anims = cfg::ship::multiAnimMap[cfg::profil::Faction::toString(cfg::profil::faction)];
        _anims.curAnim.setPosition(static_cast<float>(cfg::game::width / 2 - 337),
                                   static_cast<float>(cfg::game::height / 2 - 237));
        _gui[ProfilWidgets::MainMenuHead].setPosition(CEGUI::UVector2(CEGUI::UDim(0, 50), CEGUI::UDim(0, -24)));
        _gui[ProfilWidgets::OptionsHead].setPosition(CEGUI::UVector2(CEGUI::UDim(0, 4), CEGUI::UDim(0, -57)));
        _gui[ProfilWidgets::OptionsRealHead].setPosition(CEGUI::UVector2(CEGUI::UDim(0, 309), CEGUI::UDim(0, -15)));
    }

    void ProfilScene::__configure() noexcept
    {
        auto start = __setGUI();
        if (start) {
            _evtMgr.emit<gutils::evt::PlayMusic>(Configuration::Music::MarketInstrumentalAmb, true);
            __subscribeEvents();
            __setText();
            __getData();
            __setData();
            _settings.init();
            _bg.setTexture(Configuration::textures.get(Configuration::Sprite::BGProfil));
        }
    }

    void ProfilScene::__cancelMainMenuWindow() noexcept
    {
        _gui[ProfilWidgets::MainMenu].setVisible(false);
    }

    void ProfilScene::__mainMenuWindow() noexcept
    {
        _gui[ProfilWidgets::MainMenu].setVisible(true);
    }

    void ProfilScene::__checkResume(const sf::Keyboard::Key &key) noexcept
    {
        //TODO: We need to do more checks when we will have shop and other window
        if (key == sf::Keyboard::Escape && !_pause && !_gui[ProfilWidgets::MainMenu].isDisabled()) {
            pause();
        } else if (key == sf::Keyboard::Escape && _pause && !_gui[ProfilWidgets::MainMenu].isDisabled()) {
            resume();
        }
    }

    //! Public member functions
    void ProfilScene::enter() noexcept
    {
        showEnterScene();
        __configure();
    }

    void ProfilScene::resume() noexcept
    {
        showResumeScene();
        __cancelMainMenuWindow();
        _pause = !_pause;
    }

    void ProfilScene::pause() noexcept
    {
        showPauseScene();
        __mainMenuWindow();
        _pause = !_pause;
    }

    void ProfilScene::draw() noexcept
    {
        _win.draw(_bg);
        _win.draw(_anims.curAnim);
    }

    void ProfilScene::leave() noexcept
    {
        if (_gui.sheet != nullptr)
            GUIManager::destroySheet(*_gui.sheet);
        _pause = false;
        showLeavingScene();
    }

    void ProfilScene::update(double timeSinceLastFrame) noexcept
    {
        sf::Time t1 = sf::seconds(static_cast<float>(timeSinceLastFrame));
        _anims.update(t1);
    }

    bool ProfilScene::keyPressed(const gutils::evt::KeyPressed &evt) noexcept
    {
        __checkResume(evt.key);
        return true;
    }

    bool ProfilScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool ProfilScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool ProfilScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool ProfilScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void ProfilScene::receive([[maybe_unused]] const gutils::evt::ResetText &evt) noexcept
    {
        __setText();
    }


}
