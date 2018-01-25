//
// Created by doom on 11/01/18.
//

#ifndef RTYPE_SERVER_TCPPACKETREADER_HPP
#define RTYPE_SERVER_TCPPACKETREADER_HPP

#include <queue>
#include <functional>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <rtype/protocol/Protocol.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

template <typename Packets>
class TCPProtocolConnection : public boost::enable_shared_from_this<TCPProtocolConnection<Packets>>
{
    static constexpr const size_t chunkSize = 2;
    using ThisType = TCPProtocolConnection<Packets>;
    using Shared = boost::enable_shared_from_this<ThisType>;

public:
    template <typename ...Args>
    static auto makeShared(Args &&...args) noexcept
    {
        return boost::shared_ptr<ThisType>(new ThisType(std::forward<Args>(args)...));
    }

    using Unformatter = proto::Unformatter<Packets>;
    using Formatter = proto::Formatter<Packets>;
    using Packet = typename Unformatter::Packet;

    explicit TCPProtocolConnection(tcp::socket &&sock) noexcept : _sock(std::move(sock))
    {
    }

private:
    void _handleRead(size_t readBytes, const boost::system::error_code &ec)
    {
        if (ec) {
            _cb(ec);
        } else {
            _used += readBytes;
            if (_used > sizeof(size_t)) {
                size_t packetSize = _uf.unserializeSize(_buff);
                if (_buff.size() - sizeof(size_t) >= packetSize) {
                    auto start = _buff.begin() + sizeof(size_t);
                    _read.push(_uf.unserialize(proto::BufferSpan(start, start + packetSize)));
                    size_t consumedSize = sizeof(size_t) + packetSize;
                    _used -= consumedSize;
                    _buff.erase(_buff.begin(), _buff.begin() + consumedSize);
                    _cb({});
                    return;
                }
            }
            _doRead();
        }
    }

    void _doRead() noexcept
    {
        _buff.resize(_used + chunkSize);
        _sock.async_receive(asio::buffer(_buff.data() + _used, chunkSize),
                            boost::bind(&TCPProtocolConnection::_handleRead, Shared::shared_from_this(),
                                        asio::placeholders::bytes_transferred,
                                        asio::placeholders::error));
    }

public:
    template <typename Functor>
    void asyncRead(Functor &&f) noexcept
    {
        _cb = std::forward<Functor>(f);
        _doRead();
    }

    template <typename PacketT, typename Functor>
    auto asyncWrite(const PacketT &toSend, Functor &&f) noexcept
    {
        _fmt.serialize(toSend);
        _fmt.prefixSize();
        auto buf = _fmt.extract();
        return _sock.async_write_some(asio::buffer(buf.data(), buf.size()), std::forward<Functor>(f));
    }

    template <typename PacketT>
    auto write(const PacketT &toSend, boost::system::error_code &ec)
    {
        _fmt.serialize(toSend);
        _fmt.prefixSize();
        auto buf = _fmt.extract();
        return _sock.write_some(asio::buffer(buf.data(), buf.size()), ec);
    }

    size_t available() const noexcept
    {
        return _read.size();
    }

    Packet pop() noexcept
    {
        Packet ret = _read.front();
        _read.pop();
        return ret;
    }

    tcp::socket &socket() noexcept
    {
        return _sock;
    }

    const tcp::socket &socket() const noexcept
    {
        return _sock;
    }

private:
    Formatter _fmt;
    Unformatter _uf;
    std::function<void(const boost::system::error_code &ec)> _cb;
    size_t _used{0};
    proto::Buffer _buff;
    std::queue<Packet> _read;
    tcp::socket _sock;
};

#endif //RTYPE_SERVER_TCPPACKETREADER_HPP
