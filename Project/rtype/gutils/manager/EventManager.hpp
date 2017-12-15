//
// Created by roman sztergbaum on 13/09/2017.
//

#ifndef GUTILS_EVENTMANAGER_HPP
#define GUTILS_EVENTMANAGER_HPP

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <utils/NonCopyable.hpp>
#include <utils/Singleton.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>

namespace gutils
{
    /**
     * @brief The event manager works with type-erasure, in order to stock different template type.
     */
    struct BaseEventCallbackWrapper
    {
        virtual ~BaseEventCallbackWrapper() = default;
        virtual void operator()(const InsideEvents *event) const = 0;
    };

    template <typename TEvent>
    class EventCallbackWrapper : public BaseEventCallbackWrapper
    {
    public:
        explicit EventCallbackWrapper(const std::function<void(const TEvent &)> &callback) : _callback(callback)
        {
        }

        void operator()(const InsideEvents *event) const override
        {
            _callback(*(static_cast<const TEvent *>(event)));
        }

    private:
        std::function<void(const TEvent &)> _callback;
    };

    //! Type-Erasure again
    struct BaseReceiver : private utils::NonCopyable
    {
        ~BaseReceiver() override = default;
    };

    template <typename Derived>
    struct Receiver : public BaseReceiver
    {
    };

    class EventManager
    {
    public:
        template <typename TEvent, typename TReceiver>
        void subscribe(TReceiver &receiver) noexcept
        {
            const EventTypeID familyId = __getTypeId<TEvent>();
            BaseReceiver &base = receiver;
            auto pair = std::make_pair(&base,
                                       std::make_shared<EventCallbackWrapper<TEvent>>([&receiver](const auto &ev) {
                                           receiver.receive(ev);
                                       }));
            _receivers[familyId].emplace_back(std::move(pair));
        };

        template <typename TEvent, typename ... Args>
        void emit(Args &&... args) noexcept
        {
            TEvent event(std::forward<Args>(args)...);
            const EventTypeID familyId = __getTypeId<TEvent>();

            std::for_each(begin(_receivers[familyId]), end(_receivers[familyId]), [&event](const auto &pr) {
                //! Otherwise MSVC ambiguous call...
                pr.second->operator()(&event);
            });
        };

    private:
        using EventTypeID = size_t;

        EventTypeID __generateTypeId() noexcept
        {
            static EventTypeID cur{0};

            return cur++;
        }

        template <typename T>
        EventTypeID __getTypeId() noexcept
        {
            static EventTypeID id{__generateTypeId()};

            return id;
        }

    private:
        using CbPtr = std::shared_ptr<BaseEventCallbackWrapper>;
        using ReceiversVector = std::vector<std::pair<BaseReceiver *, CbPtr>>;
        using ReceiversMap = std::unordered_map<EventTypeID, ReceiversVector>;
        ReceiversMap _receivers;
    };
}

#endif //GUTILS_EVENTMANAGER_HPP