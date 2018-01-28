//
// Created by milerius on 21/01/18.
//

#ifndef RTYPE_TIMER_HPP
#define RTYPE_TIMER_HPP

#include <chrono>
#include <string>

namespace rtype::utils
{
    class StopWatch
    {
    public:
        void start() noexcept
        {
            _start = std::chrono::system_clock::now();
            _run = true;
        }

        void stop() noexcept
        {
            _run = false;
        }

        bool isRunning() noexcept
        {
            return _run;
        }

        std::string elapsedStr() noexcept
        {
            std::stringstream oss;
            namespace chrono = std::chrono;
            long sec = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - _start).count();
            auto minute = sec / 60;
            if (minute < 10)
                oss << "0";
            oss << minute << ":";
            sec = sec - minute * 60;
            if (sec < 10)
                oss << "0";
            oss << sec;
            _lastTime = oss.str();
            return _lastTime;
        }

        std::chrono::microseconds elapsedMicro()
        {
            if (!_run)
                return _uElapsed;
            _uElapsed = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::system_clock::now() - _start);
            return _uElapsed;
        }

        std::chrono::milliseconds elapsedMilli()
        {
            if (!_run)
                return std::chrono::duration_cast<std::chrono::milliseconds>(_uElapsed);
            _uElapsed = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::system_clock::now() - _start);
            return std::chrono::duration_cast<std::chrono::milliseconds>(_uElapsed);
        }

        std::chrono::milliseconds elapsedSec()
        {
            if (!_run)
                return std::chrono::duration_cast<std::chrono::seconds>(_uElapsed);
            _uElapsed = std::chrono::duration_cast<std::chrono::microseconds>
                    (std::chrono::system_clock::now() - _start);
            return std::chrono::duration_cast<std::chrono::seconds>(_uElapsed);
        }

        void reset() noexcept
        {
            _start = std::chrono::system_clock::now();
        }

    private:
        std::chrono::time_point<std::chrono::system_clock> _start;
        std::chrono::microseconds _uElapsed;
        std::string _lastTime;
        bool _run{false};
    };

    class CountDown
    {
    public:
        CountDown(gutils::EventManager &evtMgr, std::chrono::seconds duration) noexcept :
            _evtMgr(evtMgr),
            _duration(std::move(duration))
        {
        }

        bool isRunning() const noexcept
        {
            return _run;
        }

        void start() noexcept
        {
            _run = true;
            _start = std::chrono::system_clock::now();
        }

        void stop() noexcept
        {
            _run = false;
        }

        void reset() noexcept
        {
            _duration = _originalDuration;
        }

        void update() noexcept
        {
            using namespace std::chrono_literals;
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - _start);
            if (elapsed == 1s) {
                _start = std::chrono::system_clock::now();
            }
            _duration -= elapsed;
            if (_duration <= 0s) {
                stop();
                _evtMgr.emit<gutils::evt::CountdownEnd>();
            }
        }

        long getDuration() const noexcept
        {
            return _duration.count();
        }

    private:
        bool _run{false};
        gutils::EventManager &_evtMgr;
        std::chrono::seconds _duration;
        std::chrono::seconds _originalDuration{_duration};
        std::chrono::time_point<std::chrono::system_clock> _start;
    };
}

#endif //RTYPE_TIMER_HPP
