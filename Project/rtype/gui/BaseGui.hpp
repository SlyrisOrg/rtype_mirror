//
// Created by milerius on 03/11/17.
//

#ifndef RTYPE_BASEGUI_HPP
#define RTYPE_BASEGUI_HPP

#include <rtype/core/SafeCEGUI.hpp>

namespace rtype
{
    /**
     * @struct BaseGUI
     * 
     * @brief mini wrapper around cegui, contains operator helpers for manipulating cegui::window
     * @details it's not a mistake that is a structure. see GUIManager implementation.
     */
    template <size_t Size>
    struct BaseGUI
    {
        CEGUI::Window &operator[](size_t idx)
        {
            if (unlikely(idx > widgets.size()))
                throw std::out_of_range("idx > widgets.size()");
            return *widgets[idx];
        }

        const CEGUI::Window &operator[](size_t idx) const
        {
            if (unlikely(idx > widgets.size()))
                throw std::out_of_range("idx > widgets.size()");
            return *widgets[idx];
        }

        CEGUI::Window *sheet{nullptr};
        std::array<CEGUI::Window *, Size> widgets{{nullptr}};
    };

    struct GuiReplaceText
    {
        CEGUI::Window* widget;
        std::string search;
        std::string replace;
    };

    struct GuiProperty
    {
        CEGUI::Window* widget;
        std::string property;
        std::string value;
    };
}

#endif //RTYPE_BASEGUI_HPP
