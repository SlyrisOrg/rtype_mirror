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
    //! Type-Erasure
    struct BaseEventCallbackWrapper
    {
        virtual ~BaseEventCallbackWrapper() = default;
        virtual void operator()(const InsideEvents *event) = 0;
    };

    template <typename TEvent>
    struct EventCallbackWrapper : BaseEventCallbackWrapper
    {
        explicit EventCallbackWrapper(const std::function<void(const TEvent &)> &callback) : callback(callback)
        {
        }

        void operator()(const InsideEvents *event) override
        {
            callback(*(static_cast<const TEvent *>(event)));
        }

        std::function<void(const TEvent &)> callback;
    };

    //! Type-Erasure again
    class BaseReceiver : public utils::NonCopyable
    {
    public:
        ~BaseReceiver() override = default;
    };

    template <typename Derived>
    class Receiver : public BaseReceiver
    {
    public:
    };

    class EventManager
    {
    public:
        template <typename TEvent, typename TReceiver>
        void subscribe(TReceiver &receiver)
        {
            const TypeId familyId = __getTypeId<TEvent>();
            BaseReceiver &base = receiver;
            auto pair = std::make_pair(&base,
                                       std::make_shared<EventCallbackWrapper<TEvent>>([&receiver](const auto &ev) {
                                           receiver.receive(ev);
                                       }));
            _receivers[familyId].emplace_back(std::move(pair));
        };

        template <typename TEvent, typename ... Args>
        void emit(Args &&... args)
        {
            TEvent event(std::forward<Args>(args)...);
            const TypeId familyId = __getTypeId<TEvent>();

            std::for_each(begin(_receivers[familyId]), end(_receivers[familyId]), [&event](const auto &pr) {
                //! Otherwise MSVC ambiguous call...
                pr.second->operator()(&event);
            });
        };

    private:
        using TypeId = size_t;

        TypeId __generateTypeId()
        {
            static TypeId cur{0};

            return cur++;
        }

        template <typename T>
        TypeId __getTypeId()
        {
            static TypeId id{__generateTypeId()};

            return id;
        }

    private:
        using CbPtr = std::shared_ptr<BaseEventCallbackWrapper>;
        using Subjects = std::vector<std::pair<BaseReceiver *, CbPtr>>;
        using Receivers = std::unordered_map<TypeId, Subjects>;
        Receivers _receivers;
    };
}

#endif //GUTILS_EVENTMANAGER_HPP