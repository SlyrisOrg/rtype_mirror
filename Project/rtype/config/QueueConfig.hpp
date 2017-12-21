//
// Created by zouz on 21/12/17.
//

#ifndef RTYPE_QUEUECONFIG_HPP
#define RTYPE_QUEUECONFIG_HPP

#include <rtype/scenes/private/ForwardEnumQueue.hpp>

namespace cfg::queue
{

    static inline constexpr const char *queueLayout = "RTQueue.layout";

    //! Widget utils
    static inline constexpr const unsigned int nbWidgets = rtype::QueueWidgets::size();
}

#endif //RTYPE_QUEUECONFIG_HPP
