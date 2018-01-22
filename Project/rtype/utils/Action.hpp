//
// Created by milerius on 10/01/18.
//

#ifndef RTYPE_ACTION_HPP
#define RTYPE_ACTION_HPP

#include <functional>
#include <unordered_map>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <utils/NonCopyable.hpp>

namespace rtype
{
    class Action
    {
    public:
        enum Type
        {
            RealTime = 1,
            Pressed = 1 << 1,
            Released = 1 << 2,
        };

        friend Type operator|(Type one, Type two) noexcept
        {
            return static_cast<Type>(static_cast<int>(one) | static_cast<int>(two));
        }

        Action(const sf::Keyboard::Key &key, Type type = Type::RealTime | Type::Pressed) noexcept : type(type)
        {
            event.type = sf::Event::EventType::KeyPressed;
            event.key.code = key;
        }

        Action(const sf::Mouse::Button &button, Type type = Type::RealTime | Type::Pressed) noexcept : type(type)
        {
            event.type = sf::Event::EventType::MouseButtonPressed;
            event.mouseButton.button = button;
        }

        bool operator==(const sf::Event &event) const noexcept
        {
            bool res = false;

            switch (event.type) {
                case sf::Event::EventType::KeyPressed:
                    if (type & Type::Pressed && event.type == sf::Event::EventType::KeyPressed)
                        res = event.key.code == event.key.code;
                    break;
                case sf::Event::EventType::KeyReleased:
                    if (type & Type::Released && event.type == sf::Event::EventType::KeyPressed)
                        res = event.key.code == event.key.code;
                    break;
                case sf::Event::EventType::MouseButtonPressed:
                    if (type & Type::Pressed && event.type == sf::Event::EventType::MouseButtonPressed)
                        res = event.mouseButton.button == event.mouseButton.button;
                    break;
                case sf::Event::EventType::MouseButtonReleased:
                    if (type & Type::Released && event.type == sf::Event::EventType::MouseButtonPressed)
                        res = event.mouseButton.button == event.mouseButton.button;
                    break;
                default:
                    break;
            }
            return res;
        }

        bool operator==(const Action &other) const noexcept
        {
            return type == other.type && other == event;
        }

        bool test() const noexcept
        {
            bool res = false;
            if (event.type == sf::Event::EventType::KeyPressed) {
                if (type & Type::Pressed)
                    res = sf::Keyboard::isKeyPressed(event.key.code);
            } else if (event.type == sf::Event::EventType::MouseButtonPressed) {
                if (type & Type::Pressed)
                    res = sf::Mouse::isButtonPressed(event.mouseButton.button);
            }
            return res;
        }

    public:
        sf::Event event;
        int type;
    };

    template <typename T = Action::Type>
    class ActionMap : private utils::NonCopyable
    {
    public:
        ActionMap() noexcept
        {
        }

        void add(const T &key, const Action &action) noexcept
        {
            _actionMap.emplace(key, action);
        }

        const Action &get(const T &key) const
        {
            return _actionMap.at(key);
        }

    private:
        std::unordered_map<T, Action> _actionMap;
    };

    template <typename T = Action::Type>
    class ActionTarget : private ::utils::NonCopyable
    {
    public:

        using FuncType = std::function<void(const sf::Event &, double timeSinceLastFrame)>;

        ActionTarget(const ActionMap<T> &map) noexcept : _actionMap(map)
        {
        }

        bool processEvent(const sf::Event &event) const noexcept
        {
            for (auto &pair : _eventsPoll) {
                if (_actionMap.get(pair.first) == event) {
                    pair.second(event);
                    return true;
                }
            }
            return false;
        }

        void processEvents(double timeSinceLastFrame) const noexcept
        {
            for (auto &pair : _eventsRealTime) {
                const Action &action = _actionMap.get(pair.first);
                if (action.test())
                    pair.second(action.event, timeSinceLastFrame);
            }
        }

        void setKeyCallback(const T &key, const FuncType &callback) noexcept
        {
            const Action &action = _actionMap.get(key);
            if (action.type & Action::Type::RealTime)
                _eventsRealTime.emplace_back(key, callback);
            else
                _eventsPoll.emplace_back(key, callback);
        }

        void removeKeyCallback(const T &key) noexcept
        {
            auto remove_func = [&key](const std::pair<T, FuncType> &pair) noexcept {
                return pair.first == key;
            };

            const Action &action = _actionMap.get(key);
            if (action.type & Action::Type::RealTime)
                _eventsRealTime.erase(std::remove_if(begin(_eventsRealTime), end(_eventsRealTime), remove_func));
            else
                _eventsRealTime.erase(std::remove_if(begin(_eventsPoll), end(_eventsPoll), remove_func));
        }

        void clear() noexcept
        {
            _eventsPoll.clear();
            _eventsRealTime.clear();
        }

    private:
        std::vector<std::pair<T, FuncType>> _eventsRealTime;
        std::vector<std::pair<T, FuncType>> _eventsPoll;
        const ActionMap<T> &_actionMap;
    };
}

#endif //RTYPE_ACTION_HPP
