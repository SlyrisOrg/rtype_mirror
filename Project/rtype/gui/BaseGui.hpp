//
// Created by milerius on 03/11/17.
//

#ifndef RTYPE_BASEGUI_HPP
#define RTYPE_BASEGUI_HPP

#include <rtype/core/SafeCEGUI.hpp>

namespace rtype
{
    template <size_t size>
    struct BaseGui
    {
        CEGUI::Window &operator[](size_t idx) noexcept
        {
            assert(idx < widgets.size());
            return *widgets[idx];
        }

        const CEGUI::Window &operator[](size_t idx) const noexcept
        {
            assert(idx < widgets.size());
            return *widgets[idx];
        }

        CEGUI::Window *sheet{nullptr};
        std::array<CEGUI::Window *, size> widgets{{nullptr}};
    };
}

#endif //RTYPE_BASEGUI_HPP
