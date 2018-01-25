//
// Created by zouz on 21/12/17.
//

#include <thread>
#include <mutex>
#include <rtype/config/ProfilConfig.hpp>
#include <rtype/config/SpaceshipConfig.hpp>
#include <rtype/gui/GUIManager.hpp>
#include <rtype/scenes/private/ForwardEnumQueue.hpp>
#include <rtype/scenes/QueueScene.hpp>

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
        for (auto &&currentBorder : _bordersBg) {
            _win.draw(currentBorder);
        }

        for (auto &&curAnimArray : _anims) {
            _win.draw(curAnimArray.curAnim);
        }
    }

    void QueueScene::leave() noexcept
    {
        _io->stop();
        _networkThread.join();
        _bordersBg.clear();
        _anims.clear();
        _current = 1;
        _currentPlayerBorder = QueueWidgets::BorderTwo;
        _currentPlayerButton = QueueWidgets::PlayerTwoButton;
        _currentLabelPlayer = QueueWidgets::LabelPlayerTwo;
        _currentLabelPlayerNickname = QueueWidgets::LabelPlayerTwoNickname;
        if (_gui.sheet != nullptr)
            GUIManager::destroySheet(*_gui.sheet);
        showLeavingScene();
        __queueMusicStop();
    }

    void QueueScene::update(double timeSinceLastFrame) noexcept
    {
        sf::Time t1 = sf::seconds(static_cast<float>(timeSinceLastFrame));
        for (auto &&curAnimArray : _anims) {
            curAnimArray.update(t1);
        }
        if (_stopWatch.isRunning()) {
            _gui[QueueWidgets::TimerText].setText(_stopWatch.elapsedStr());
        }
        if (_countDown.isRunning()) {
            _countDown.update();
            _gui[QueueWidgets::TimerText].setText("Game Start: " + std::to_string(_countDown.getDuration()));
        }
    }

    void QueueScene::receive(const gutils::evt::AddPlayerQueue &evt) noexcept
    {
        _log(lg::Debug) << "player info:\nnickname: " << evt.info.nickName << std::endl;
        matchmaking::PlayerInfo info = evt.info;
        using namespace std::string_literals;
        auto func = [this, &info]() {
            _gui[_currentPlayerButton].setProperty("Image", "RTButtonBar/tab_h");
            _gui[_currentLabelPlayerNickname].setText(info.nickName);
            _gui[_currentLabelPlayer].setText(
                "Level "s + std::to_string(info.lvl) + " " + rtype::Faction::toString(info.faction));
            rtype::MultiAnim &res = _anims.emplace_back(
                cfg::ship::multiAnimMap[rtype::Faction::toString(info.faction)]);
            res.curAnim.setScale(res.curAnim.getScale() / 1.8f);
            res.curAnim.setPosition(_gui[_currentPlayerBorder].getXPosition().d_offset + 40,
                                    _gui[_currentPlayerBorder].getYPosition().d_offset + 160);
            _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::JoinSuccess);
        };

        switch (cfg::player::mode) {
            case matchmaking::Duo:
                func();
                ++_current;
                break;
            case matchmaking::Trio:
            case matchmaking::Quatuor:
                func();
                ++_current;
                ++_currentLabelPlayerNickname;
                ++_currentLabelPlayer;
                ++_currentPlayerBorder;
                ++_currentPlayerButton;
                break;
            case matchmaking::Solo:
            case matchmaking::None:
            default:
                break;
        }
        if (_current == _required) {
            _log(lg::Debug) << "Match starting..." << std::endl;
            for (auto &&curAnim : _anims) {
                curAnim.curAnim.setFrame(0);
            }
            _stopWatch.stop();
            _countDown.start();
            _gui[QueueWidgets::TimerText].setProperty("Font", "LaconicLight-18");
            _gui[QueueWidgets::TimerText].setText("Game Start: 10");
        }
    }

    void QueueScene::receive([[maybe_unused]] const gutils::evt::JoinQueue &evt) noexcept
    {
        using namespace std::string_literals;
        _log(lg::Debug) << "Player One join the queue" << std::endl;
        _stopWatch.start();
        _gui[QueueWidgets::PlayerOneButton].setProperty("Image", "RTButtonBar/tab_h");
        _gui[QueueWidgets::LabelPlayerOneNickname].setText(cfg::profil::nickname);
        _gui[QueueWidgets::LabelPlayerOne].setText(
            "Level "s + cfg::profil::level + " " + cfg::profil::Faction::toString(cfg::profil::faction));
        rtype::MultiAnim &res = _anims.emplace_back(
            cfg::ship::multiAnimMap[cfg::profil::Faction::toString(cfg::profil::faction)]);
        res.curAnim.setScale(res.curAnim.getScale() / 1.8f);
        res.curAnim.setPosition(_gui[QueueWidgets::BorderOne].getXPosition().d_offset + 40,
                                _gui[QueueWidgets::BorderOne].getYPosition().d_offset + 160);
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::JoinSuccess);
    }

    bool QueueScene::keyPressed([[maybe_unused]]const gutils::evt::KeyPressed &evt) noexcept
    {
        __gotoLobbyScene(evt);
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
            _required = cfg::player::mode + 1;
            __resetUi();
            _countDown.reset();
            __queueMusicStart();
            __subscribeEvents();
            __initTextures();
            __setupNetwork();
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

        _gui[QueueWidgets::DiscardButton].subscribeEvent(pb::EventClicked,
                                                         evt::Subscriber(&Qs::__goBackToProfil, this));
    }

    void QueueScene::__goBackToProfil() noexcept
    {
        _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
        _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
    }

    void QueueScene::__resetUi() noexcept
    {
        _gui[QueueWidgets::Timer].setPosition(_gui[QueueWidgets::Timer].getPosition());
        {
            std::scoped_lock<std::mutex> lock{_mutex};
            switch (cfg::player::mode) {
                case matchmaking::Duo:
                    _gui[QueueWidgets::BorderFour].hide();
                    _gui[QueueWidgets::BorderThree].hide();
                    break;
                case matchmaking::Trio:
                    _gui[QueueWidgets::BorderFour].hide();
                    _gui[QueueWidgets::BorderTwo].setXPosition(
                        _gui[QueueWidgets::BorderTwo].getXPosition() - CEGUI::UDim{0, 210});
                    _gui[QueueWidgets::BorderOne].setXPosition(
                        _gui[QueueWidgets::BorderOne].getXPosition() - CEGUI::UDim{0, 210});
                    _gui[QueueWidgets::BorderThree].setXPosition(
                        _gui[QueueWidgets::BorderThree].getXPosition() - CEGUI::UDim{0, 210});
                    break;
                case matchmaking::Quatuor:
                    break;
                case matchmaking::Solo:
                case matchmaking::None:
                    break;
                default:
                    break;
            }
        }
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

        float x = 0;
        float y = 0;
        float offset = 0;

        auto setData = [&x, &y, &offset, this](size_t from, size_t to) {
            x = _gui[from].getPosition().d_x.d_offset + 6.0f;
            y = _gui[from].getPosition().d_y.d_offset + 12.0f;
            offset = _gui[to].getPosition().d_x.d_offset
                     - _gui[from].getPosition().d_x.d_offset;
        };

        if (cfg::player::mode == matchmaking::Duo || cfg::player::mode == matchmaking::Trio) {
            setData(QueueWidgets::BorderTwo, QueueWidgets::BorderOne);
        } else if (cfg::player::mode == matchmaking::Quatuor) {
            setData(QueueWidgets::BorderFour, QueueWidgets::BorderTwo);
        }

        sf::Vector2f pos(x, y);
        for (unsigned int i = 0; i < (cfg::player::mode + 1); ++i) {
            auto &spr = _bordersBg.emplace_back();
            spr.setTexture(Configuration::textures.get(Configuration::Sprite::BorderBG));
            spr.setPosition(pos);
            pos.x += offset;
        }
    }

    void QueueScene::__gotoLobbyScene(const gutils::evt::KeyPressed &evt) noexcept
    {
        if (evt.key == sf::Keyboard::F1) {
            _evtMgr.emit<gutils::evt::ChangeScene>(rtype::Scene::Lobby);
        }
    }

    void QueueScene::__setupNetwork() noexcept
    {
        _io.reset(new asio::io_service());
        _networkThread = std::thread([this]() {
            logging::Logger log{"QueueNetwork", logging::Debug};
            asio::signal_set sigSet{*_io, SIGINT, SIGTERM};
            sigSet.async_wait([this, &log]([[maybe_unused]] const sys::error_code &error, int sig) {
                {
                    std::scoped_lock<std::mutex> lock{this->_mutex};
                    log(lg::Debug) << "Stopping: caught signal " << sig << std::endl;
                }
                _io->stop();
            });
            tcp::endpoint endpoint{asio::ip::address::from_string("127.0.0.1"), 31337};
            TCPClient client{*_io, endpoint, log, _mutex, _evtMgr};
            _io->run();
        });
    }

    void QueueScene::receive([[maybe_unused]] const gutils::evt::CountdownEnd &evt) noexcept
    {
        _log(lg::Info) << "Starting main game..." << std::endl;
        _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Game);
    }
}

