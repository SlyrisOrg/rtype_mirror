//
// Created by milerius on 24/01/18.
//

#include <rtype/gui/GUIManager.hpp>
#include <rtype/scenes/GameScene.hpp>

namespace rtype
{
    void GameScene::enter() noexcept
    {
        _configure();
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
        _ioThread.release();
    }

    void GameScene::update([[maybe_unused]] double timeSinceLastFrame) noexcept
    {
        auto show = [](auto &&v, auto &&log) {
            using Decayed = std::decay_t<decltype(v)>;
            if constexpr (!std::is_same_v<std::monostate, Decayed>) {
                log(lg::Debug) << "Got a packet of type " << Decayed::className() << std::endl;
            }
        };
        net::details::GamePackets packet;
        while (_ioThread->queue().pop(packet)) {
            auto visitor = meta::makeVisitor([this, &show](game::Welcome &welcomePacket) {
                show(welcomePacket, _log);
                game::Authenticate auth{cfg::game::tokenSession};
                _ioThread->sendPacket(auth);
            }, [this, &show](auto &&v) {
                show(v, _log);
            });
            std::visit(visitor, packet);
        }
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

    void GameScene::_configure()
    {
        auto start = _setGUI();
        if (start) {
            _configureNetwork();
        }
    }

    bool GameScene::_setGUI() noexcept
    {
        using namespace cfg::play;
        return GUIManager::setGUI<UIWidgets, nbWidgets>(UILayout, _gui, _log);
    }

    void GameScene::_configureNetwork()
    {
        tcp::endpoint endpoint{asio::ip::address::from_string("192.168.0.31"), _remoteGamePort};
        _ioThread = std::make_unique<net::ClientIOThread>(endpoint);
        _ioThread->run();

        // TODO: Connection
    }
}