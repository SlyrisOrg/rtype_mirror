//
// Created by doom on 25/01/18.
//

#ifndef RTYPE_SERVER_IOTHREAD_HPP
#define RTYPE_SERVER_IOTHREAD_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <rtype/netproto/TCPProtocolConnection.hpp>
#include <rtype/protocol/Game.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

template <typename T>
class Queue
{
public:
    bool pop(T &item)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        if (_queue.empty()) {
            return false;
        }
        item = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    template <typename U>
    void push(U &&item)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _queue.push(std::forward<U>(item));
        lock.unlock();
        _cond.notify_one();
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
};

namespace rtype
{
    template <typename QueuedT>
    class IOThread
    {
    public:
        ~IOThread() noexcept
        {
            if (_thread.joinable()) {
                _thread.join();
            }
        }

    protected:
        template<typename Func>
        void run(Func &&func)
        {
            _thread = std::thread([this, fc = std::forward<Func>(func)]() {
                asio::signal_set sigSet{_io, SIGINT, SIGTERM};

                sigSet.async_wait([this]([[maybe_unused]] const boost::system::error_code &ec,
                                         [[maybe_unused]] int sig) {
                    _io.stop();
                });
                fc();
                _io.run();
            });
        }

    public:
        bool stopped() const noexcept
        {
            return _io.stopped();
        }

        auto &queue() noexcept
        {
            return _queue;
        }

    protected:
        Queue<QueuedT> _queue;
        asio::io_service _io;
        std::thread _thread;
    };
}

#endif //RTYPE_SERVER_IOTHREAD_HPP
