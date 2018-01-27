//
// Created by milerius on 27/01/18.
//

#ifndef RTYPE_CLIENTIOTHREAD_HPP
#define RTYPE_CLIENTIOTHREAD_HPP

#include <rtype/protocol/Game.hpp>
#include <rtype/netproto/IOThread.hpp>
#include <log/Logger.hpp>

namespace net
{
    namespace details
    {
        using GamePackets = meta::list::Convert<meta::list::PushFront<game::Packets, std::monostate>, std::variant>;
    }

    class ClientIOThread : public rtype::IOThread<details::GamePackets>
    {
    private:
        using TCPProtoConn = TCPProtocolConnection<game::Packets>;
        boost::shared_ptr<TCPProtoConn> _conn{nullptr};

    private:
        void _readPackets(const boost::system::error_code &ec)
        {
            if (!ec) {
                _log(logging::Debug) << __PRETTY_FUNCTION__ << std::endl;
                _log(logging::Debug) << _conn->available() << std::endl;
                while (_conn->available() > 0) {
                    auto packet = _conn->pop();
                    _queue.push(std::move(packet));
                }
                _conn->asyncRead(boost::bind(&ClientIOThread::_readPackets, this, boost::asio::placeholders::error));
            } else {
                _log(logging::Error) << ec.message() << std::endl;
            }
        }

    public:
        ClientIOThread(tcp::endpoint ep) noexcept : _endpoint(std::move(ep)), _sock(_io)
        {
        }

        void run() noexcept
        {
            IOThread::run([this]() {
                boost::system::error_code ec;
                _sock.connect(_endpoint, ec);
                if (ec) {
                    _log(logging::Debug) << ec.message() << std::endl;
                } else {
                    _log(logging::Info) << "Connect Success" << std::endl;
                }
                _conn = TCPProtoConn::makeShared(std::move(_sock));
                _conn->asyncRead(boost::bind(&ClientIOThread::_readPackets, this, boost::asio::placeholders::error));
            });
        }

        template<typename Packet>
        void sendPacket(const Packet &packet)
        {
            boost::system::error_code ec;
            _log(logging::Debug) << "size_t: " <<  _conn->write(packet, ec) << std::endl;
            if (ec) {
                _log(logging::Debug) << "error: " << ec.message() << std::endl;
            }
        }

    private:
        logging::Logger _log{"ClientIOThread", logging::Debug};
        tcp::endpoint _endpoint;
        tcp::socket _sock;
    };
}

#endif //RTYPE_CLIENTIOTHREAD_HPP
