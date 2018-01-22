//
// Created by zouz on 21/12/17.
//

#ifndef RTYPE_QUEUESCENE_HPP
#define RTYPE_QUEUESCENE_HPP

#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/QueueConfig.hpp>
#include <rtype/config/PlayerConfig.hpp>
#include <rtype/netproto/TCPProtocolConnection.hpp>
#include <rtype/utils/Timer.hpp>

namespace asio = boost::asio;
namespace sys = boost::system;
using tcp = asio::ip::tcp;

namespace rtype
{

    class TCPClient
    {
    private:
        using TCPProtoConnect = TCPProtocolConnection<matchmaking::Packets>;

    public:
        TCPClient(asio::io_service &io, tcp::endpoint &endpoint, logging::Logger &log, std::mutex &mutex,
                  gutils::EventManager &evtMgr) noexcept
            : _io(io),
              _socket(_io),
              _log(log),
              _mutex(mutex),
              _evtMgr(evtMgr)
        {
            connect(endpoint);
        }

        void connect(tcp::endpoint &endpoint) noexcept
        {
            _socket.async_connect(endpoint,
                                  boost::bind(&TCPClient::handleConnect,
                                              this,
                                              boost::asio::placeholders::error));
        }

        void processPlayer(const boost::system::error_code &error)
        {
            using Packet = typename TCPProtoConnect::Packet;
            if (!error) {
                Packet packet = _protoConnect->pop();
                if (std::holds_alternative<matchmaking::PlayerInfo>(packet)) {
                    const auto &player = std::get<matchmaking::PlayerInfo>(packet);
                    _evtMgr.emit<gutils::evt::AddPlayerQueue>(std::move(player));
                    _log(lg::Debug) << "before decrementation need: " << _need << std::endl;
                    if (--_need > 0) {
                        _log(lg::Debug) << "after decrementation need: " << _need << std::endl;
                        _protoConnect->asyncRead(boost::bind(&TCPClient::processPlayer,
                                                             this,
                                                             boost::asio::placeholders::error));
                    }
                }
            } else {
                _log(lg::Debug) << "read error: " << error.message() << std::endl;
            }
        }

        void matchFound(const boost::system::error_code &error)
        {
            using Packet = typename TCPProtoConnect::Packet;
            if (!error) {
                _log(lg::Debug) << "Je suis un animal" << std::endl;
                Packet p = _protoConnect->pop();
                if (std::holds_alternative<matchmaking::MatchFound>(p)) {
                    {
                        std::scoped_lock<std::mutex> lock{_mutex};
                        _log(lg::Debug) << "Match Found !! need before lambda call -> " << _need << std::endl;
                        if (_need > 0) {
                            _protoConnect->asyncRead(boost::bind(&TCPClient::processPlayer,
                                                                 this,
                                                                 boost::asio::placeholders::error));
                        }
                    }
                }
            } else {
                _log(lg::Debug) << "read error: " << error.message() << std::endl;
            }
        }

        void sendMode(const boost::system::error_code &error)
        {
            if (!error) {
                {
                    std::scoped_lock<std::mutex> lock{_mutex};
                    _log(lg::Info) << "queue join success" << std::endl;
                    _evtMgr.emit<gutils::evt::JoinQueue>();
                    _protoConnect->asyncRead(boost::bind(&TCPClient::matchFound,
                                                         this,
                                                         boost::asio::placeholders::error));
                }
            } else {
                {
                    std::scoped_lock<std::mutex> lock{_mutex};
                    _log(lg::Info) << "queue join error: " << error.message() << std::endl;
                }
            }
        }

        void sendJWTCB(const boost::system::error_code &error)
        {
            if (!error) {
                {
                    std::scoped_lock<std::mutex> lock{_mutex};
                    _log(lg::Info) << "authenticate success" << std::endl;
                    matchmaking::QueueJoin joinPacket{cfg::player::mode};
                    _protoConnect->asyncWrite(joinPacket, boost::bind(&TCPClient::sendMode,
                                                                      this,
                                                                      boost::asio::placeholders::error));
                }
            } else {
                {
                    std::scoped_lock<std::mutex> lock{_mutex};
                    _log(lg::Info) << "authenticate error: " << error.message() << std::endl;
                }
            }
        }

        void handleConnect(const boost::system::error_code &error)
        {
            if (!error) {
                {
                    std::scoped_lock<std::mutex> lock{_mutex};
                    _log(lg::Info) << "Connect Success" << std::endl;
                    _protoConnect = std::make_unique<TCPProtoConnect>(std::move(_socket));

                    matchmaking::Authenticate auth{cfg::game::tokenSession};
                    _protoConnect->asyncWrite(auth, boost::bind(&TCPClient::sendJWTCB,
                                                                this,
                                                                boost::asio::placeholders::error));
                }
            } else {
                {
                    std::scoped_lock<std::mutex> lock{_mutex};
                    _log(lg::Info) << "Connect Error: " << error.message() << std::endl;
                }
            }
        }

    private:
        asio::io_service &_io;
        tcp::socket _socket;
        logging::Logger &_log;
        std::mutex &_mutex;
        std::unique_ptr<TCPProtoConnect> _protoConnect{nullptr};
        gutils::EventManager &_evtMgr;
        int _need{static_cast<int>(cfg::player::mode)};
    };

    class QueueScene final : public gutils::AScene
    {
    public:
        template <typename ...Args>
        QueueScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {
            _evtMgr.subscribe<gutils::evt::JoinQueue>(*this);
            _evtMgr.subscribe<gutils::evt::AddPlayerQueue>(*this);
        }

        ~QueueScene() noexcept
        {
            delete _io;
        };

    private:
        using QueueGui = BaseGUI<cfg::queue::nbWidgets>;

    private:
        bool __setGUI() noexcept;
        void __configure() noexcept;
        void __subscribeEvents() noexcept;
        void __goBackToProfil() noexcept;
        void __resetUi() noexcept;
        void __queueMusicStart() noexcept;
        void __queueMusicStop() noexcept;
        void __initTextures() noexcept;
        void __gotoLobbyScene(const gutils::evt::KeyPressed &evt) noexcept;
        void __setupNetwork() noexcept;

    public:
        void enter() noexcept final;
        void resume() noexcept final;
        void pause() noexcept final;
        void draw() noexcept final;
        void leave() noexcept final;
        void update(double timeSinceLastFrame) noexcept final;
        bool keyPressed(const gutils::evt::KeyPressed &evt) noexcept final;
        bool keyReleased(const gutils::evt::KeyReleased &evt) noexcept final;
        bool mouseMoved(const gutils::evt::MouseMoved &evt) noexcept final;
        bool mousePressed(const gutils::evt::MouseButtonPressed &evt) noexcept final;
        bool mouseReleased(const gutils::evt::MouseButtonReleased &evt) noexcept final;
        void receive(const gutils::evt::JoinQueue &evt) noexcept;
        void receive(const gutils::evt::AddPlayerQueue &evt) noexcept;

    private:
        std::vector<rtype::MultiAnim> _anims;
        rtype::utils::StopWatch _stopWatch;
        QueueGui _gui;
        std::vector<sf::Sprite> _bordersBg;
        std::mutex _mutex;
        sf::Sprite _bg;
        asio::io_service *_io{new asio::io_service()};
        std::thread _networkThread{};
        unsigned int _required{cfg::player::mode + 1};
        unsigned int _current{1};
        unsigned int _currentPlayerBorder{QueueWidgets::BorderTwo};
        unsigned int _currentPlayerButton{QueueWidgets::PlayerTwoButton};
        unsigned int _currentLabelPlayer{QueueWidgets::LabelPlayerTwo};
        unsigned int _currentLabelPlayerNickname{QueueWidgets::LabelPlayerTwoNickname};
    };
}

#endif //RTYPE_QUEUESCENE_HPP
