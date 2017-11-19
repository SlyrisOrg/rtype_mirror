//
// Created by szterg_r on 29/10/2017.
//

#ifndef RTYPE_CREATECONFIG_HPP
#define RTYPE_CREATECONFIG_HPP

#include <string>
#include <CEGUI/Size.h>

namespace cfg::create
{
    static inline constexpr const char *createLayout = "RTCreate.layout";
    extern std::string chooseFactionImage;
    extern std::string backButtonText;
    extern std::string createButtonText;
    extern std::string factionHeadLabel;
    extern std::string spaceshipHeadLabel;
    extern CEGUI::USize chooseFactionSize;
}

#endif //RTYPE_CREATECONFIG_HPP
