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

        std::string elaspedStr()
        {
            if (!_run)
                return _lastTime;
            std::string out;
            long sec = std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now() - _start).count();
            auto minute = sec / 60;
            if (minute < 10)
                out += "0";
            out += std::to_string(minute) + ":";
            sec = sec - minute * 60;
            if (sec < 10)
                out += "0";
            out += std::to_string(sec);
            _lastTime = out;
            return out;
        }

        void reset() noexcept
        {
            _start = std::chrono::system_clock::now();
        }

    private:
        std::chrono::time_point<std::chrono::system_clock> _start;
        std::string _lastTime;
        bool _run{false};
    };
}

#endif //RTYPE_TIMER_HPP
