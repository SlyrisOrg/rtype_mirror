//
// Created by doom on 26/01/18.
//

#ifndef RTYPE_SERVER_WEAKCALLBACK_HPP
#define RTYPE_SERVER_WEAKCALLBACK_HPP

#include <boost/weak_ptr.hpp>

namespace utils
{
    namespace details
    {
        template <typename SharedPtr, typename Function>
        class WeakCallBack
        {
        public:
            WeakCallBack(SharedPtr &&ptr, Function &&f) noexcept : _weakPtr(std::forward<SharedPtr>(ptr)),
                                                                   _f(std::forward<Function>(f))
            {
            }

            template <typename ...Args>
            void operator()(Args &&...args)
            {
                if (auto shared = _weakPtr.lock()) {
                    _f(std::forward<Args>(args)...);
                } else {
                    //Do nothing as the pointer is not valid anymore
                }
            }

        private:
            boost::weak_ptr<typename SharedPtr::element_type> _weakPtr;
            Function _f;
        };
    }

    template <typename SharedPtr, typename Function>
    auto makeWeakCallback(SharedPtr &&ptr, Function &&f)
    {
        return details::WeakCallBack<SharedPtr, Function>(std::forward<SharedPtr>(ptr), std::forward<Function>(f));
    }
}

#endif //RTYPE_SERVER_WEAKCALLBACK_HPP
