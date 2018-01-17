//
// Created by szterg_r on 30/10/2017.
//

//#include <net/CheckConnection.hpp>
#include <rtype/scenes/LoginScene.hpp>

namespace rtype
{
    //! Private Member functions
    void LoginScene::__subscribeEvents() noexcept
    {
        using namespace cfg::login;
        using fw = CEGUI::FrameWindow;
        using pb = CEGUI::PushButton;
        using evt = CEGUI::Event;
        using ls = LoginScene;

        _gui[LoginWidgets::ExitButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&ls::__pause, this));
        _gui[LoginWidgets::YesQuit].subscribeEvent(pb::EventClicked, evt::Subscriber(&ls::__leaveGame, this));
        _gui[LoginWidgets::NoQuit].subscribeEvent(pb::EventClicked, evt::Subscriber(&ls::__resume, this));
        _gui[LoginWidgets::LogButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&ls::__login, this));
        _gui[LoginWidgets::ExitWindow].subscribeEvent(fw::EventCloseClicked, evt::Subscriber(&ls::__resume, this));
        _gui[LoginWidgets::ErrorWindow].subscribeEvent(fw::EventCloseClicked, evt::Subscriber(&ls::__resume, this));
        _gui[LoginWidgets::ErrorYesQuit].subscribeEvent(pb::EventClicked, evt::Subscriber(&ls::__resume, this));
    }

    void LoginScene::__activeWindowAndTitle(CEGUI::Window &window, CEGUI::Window &title) noexcept
    {
        window.setVisible(true);
        window.activate();
        title.setPosition(CEGUI::UVector2(CEGUI::UDim(0, 212), CEGUI::UDim(0, 27)));
        reinterpret_cast<CEGUI::FrameWindow *>(&window)->getTitlebar()->setRiseOnClickEnabled(false);
    }

    void LoginScene::__errorWindow(std::string textError) noexcept
    {
        using namespace cfg::login;
        showPauseScene();
        __disableStandardWidgets();
        __activeWindowAndTitle(_gui[LoginWidgets::ErrorWindow], _gui[LoginWidgets::ErrorTitle]);
        textError.insert(0, 135, ' ');
        _gui[LoginWidgets::ErrorMultiLineEditbox].setText(textError);
        _pause = !_pause;
    }

    void LoginScene::__activeStandardWidgets() noexcept
    {
        using namespace cfg::login;
        _gui[LoginWidgets::Form].setVisible(true);
        _gui[LoginWidgets::ExitButton].enable();
        _gui[LoginWidgets::Form].enable();
    }

    void LoginScene::__cancelLeave() noexcept
    {
        using namespace cfg::login;
        _gui[LoginWidgets::ExitWindow].setVisible(false);
        _gui[LoginWidgets::ErrorWindow].setVisible(false);
        _gui[LoginWidgets::ExitWindow].deactivate();
        _gui[LoginWidgets::ErrorWindow].deactivate();
        __activeStandardWidgets();
        _pause = !_pause;
    }

    void LoginScene::__disableStandardWidgets() noexcept
    {
        using namespace cfg::login;
        _gui[LoginWidgets::Form].setVisible(false);
        _gui[LoginWidgets::ExitButton].disable();
        _gui[LoginWidgets::Form].disable();
    }

    void LoginScene::__leaveWindow() noexcept
    {
        __disableStandardWidgets();
        __activeWindowAndTitle(_gui[LoginWidgets::ExitWindow], _gui[LoginWidgets::ExitTitle]);
        _pause = !_pause;
    }

    void LoginScene::__setText() noexcept
    {
        using namespace cfg::login;
        for (auto &&cur : logLabel) {
            const auto &[widgetID, widgetTxt] = cur.second;
            _gui[widgetID].setText(widgetTxt);
        }
    }

    void LoginScene::__setImage() noexcept
    {
        using namespace cfg::login;
        GUIManager::applyCEGUIFunc(&GUIManager::setSize,
                                   std::make_pair(&_gui[LoginWidgets::AccountInformation], accountInformationSize),
                                   std::make_pair(&_gui[LoginWidgets::AccountNameLabel], accountNameLabelSize),
                                   std::make_pair(&_gui[LoginWidgets::AccountPasswordLabel], accountPasswordLabelSize));
        GUIManager::applyCEGUIFunc(&GUIManager::setProperty,
                                   GuiProperty{&_gui[LoginWidgets::AccountNameLabel], "Image", accountNameLabelImage},
                                   GuiProperty{&_gui[LoginWidgets::AccountInformation], "Image",
                                               accountInformationImage},
                                   GuiProperty{&_gui[LoginWidgets::AccountPasswordLabel], "Image",
                                               accountPasswordLabelImage});
    }

    bool LoginScene::__setGUI() noexcept
    {
        using namespace cfg::login;
        return GUIManager::setGUI<LoginWidgets, LoginWidgets::size()>(loginLayout, _gui, _log);
    }

    void LoginScene::__configure() noexcept
    {
        _win.setView(_win.getDefaultView());
        auto start = __setGUI();
        if (start) {
            _fading = true;
            _gui.sheet->setAlpha(0);
            CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
            __subscribeEvents();
            _gui[LoginWidgets::LogButton].setWantsMultiClickEvents(false);
            _gui[LoginWidgets::LogButton].setMouseAutoRepeatEnabled(false);
            _gui[LoginWidgets::AccountName].activate();
            __setImage();
            __setText();
            _evtMgr.emit<gutils::evt::PlayMusic>(Configuration::Music::BattleExtInstrumentalAmb, true);
        } else
            __leaveGame();
    }

    void LoginScene::__checkResume(const sf::Keyboard::Key &key) noexcept
    {
        if (key == sf::Keyboard::Escape && !_pause && !_fading) {
            pause();
        } else if (key == sf::Keyboard::Escape && _pause) {
            resume();
        }
    }

    void LoginScene::__checkReturnWidgets(const sf::Keyboard::Key &key) noexcept
    {
        if (key == sf::Keyboard::Return && _gui[LoginWidgets::AccountPassword].isActive()) {
            __login();
        } else if (key == sf::Keyboard::Return && _gui[LoginWidgets::ErrorWindow].isActive()) {
            __resume();
            _gui[LoginWidgets::AccountName].activate();
        } else if (key == sf::Keyboard::Return && _gui[LoginWidgets::ExitWindow].isActive()) {
            __leaveGame();
        }
    }

    void LoginScene::__checkTabWidgets(const sf::Keyboard::Key &key) noexcept
    {
        if (key == sf::Keyboard::Tab && _gui[LoginWidgets::AccountName].isActive()) {
            __tabAccount();
        } else if (key == sf::Keyboard::Tab && _gui[LoginWidgets::AccountPassword].isActive()) {
            __tabPassword();
        }
    }

    void LoginScene::__noInternet(const std::string &msg) noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ComputerError);
        __errorWindow(msg);
    }
}

namespace rtype
{
    //! CEGUI Callbacks
    void LoginScene::__resume() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        resume();
    }

    void LoginScene::__pause() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::Click);
        pause();
    }

    void LoginScene::__leaveGame() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::Click);
        _evtMgr.emit<gutils::evt::GameShutdown>();
        leave();
    }

    void LoginScene::__login() noexcept
    {
        _gui[LoginWidgets::LogButton].disable();
        //std::error_code ec = (net::checkConnection("www.google.fr")) ? RTypeErrorCode::Success : RTypeErrorCode::NoInternet;
        std::error_code ec = RTypeErrorCode::Success;
        if (ec == RTypeErrorCode::NoInternet) {
            __noInternet(ec.message());
            _gui[LoginWidgets::LogButton].enable();
            return;
        }
        auto freshAccount = false;
        const auto res = rtype::API::requestLogin(_gui[LoginWidgets::AccountName].getText().c_str(),
                                                  _gui[LoginWidgets::AccountPassword].getText().c_str(), freshAccount,
                                                  ec).wait();
        if (res == pplx::task_group_status::completed) {
            _gui[LoginWidgets::LogButton].enable();
        }
        if (!ec) {
            _log(logging::Info) << "Checking for new player ..." << std::endl;
            _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::LoginSuccess);
            if (freshAccount) {
                _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Create);
            } else {
                _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
            }
        } else {
            _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ComputerError);
            __errorWindow(ec.message());
        }
    }

    void LoginScene::__tabAccount() noexcept
    {
        _gui[LoginWidgets::AccountPassword].activate();
    }

    void LoginScene::__tabPassword() noexcept
    {
        _gui[LoginWidgets::AccountName].activate();
    }
}

namespace rtype
{
    //! Public Member functions.
    void LoginScene::enter() noexcept
    {
        showEnterScene();
        __configure();
    }

    void LoginScene::resume() noexcept
    {
        showResumeScene();
        __cancelLeave();
    }

    void LoginScene::pause() noexcept
    {
        showPauseScene();
        __leaveWindow();
    }

    void LoginScene::leave() noexcept
    {
        if (_gui.sheet != nullptr)
            GUIManager::destroySheet(*_gui.sheet);
        showLeavingScene();
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::BattleExtInstrumentalAmb);
    }

    void LoginScene::update(double timeSinceLastFrame) noexcept
    {
        if (_gui.sheet->getAlpha() < 1) {
            _gui.sheet->setAlpha(static_cast<float>(_gui.sheet->getAlpha() + (0.2f * timeSinceLastFrame)));
        }
        else
            _fading = false;
    }

    bool LoginScene::keyPressed(const gutils::evt::KeyPressed &evt) noexcept
    {
        __checkResume(evt.key);
        __checkReturnWidgets(evt.key);
        __checkTabWidgets(evt.key);
        _gui.sheet->setAlpha(1);
        if (evt.key == sf::Keyboard::F2) {
            _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Demo);
        }

        return true;
    }

    bool LoginScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return true;
    }

    bool LoginScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        _gui.sheet->setAlpha(1);
        return true;
    }

    bool LoginScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        _gui.sheet->setAlpha(1);
        return true;
    }

    bool LoginScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return true;
    }

    void LoginScene::draw() noexcept
    {
    }
}