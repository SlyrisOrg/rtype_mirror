//
// Created by szterg_r on 29/10/2017.
//

#ifndef RTYPE_CREATECONFIG_HPP
#define RTYPE_CREATECONFIG_HPP

#include <string>
#include <unordered_map>
#include <CEGUI/Size.h>
#include <rtype/utils/AnimatedSprite.hpp>
#include <rtype/scenes/private/ForwardEnumCreate.hpp>

namespace cfg::create
{
    //! Layouts
    static inline constexpr const char *createLayout = "RTCreate.layout";

    //! Property
    extern std::string chooseFactionImage;
    extern CEGUI::USize chooseFactionSize;

    //! Animations
    extern std::unordered_map<std::string, rtype::AnimInfo> animCreate;
    extern std::unordered_map<std::string, std::vector<sfutils::AnimatedSprite>> multiAnimMap;

    //! Lang
    using TCreateWidget = rtype::CW::EnumType;
    using CLabelValues = std::pair<TCreateWidget, std::string>;
    using namespace std::string_literals;
    inline std::unordered_map<std::string, CLabelValues> createLabel
        {
            {
                "backbutton"s, {TCreateWidget::BackButton, "BACK"s}
            },
            {
                "createbutton"s, {TCreateWidget::CreateButton, "CREATE"s}
            },
            {
                "factionheadlabel"s, {TCreateWidget::FactionHeadLabel, "The \"Bheet\" faction"s}
            },
            {
                "factionheadspaceshiplabel"s, {TCreateWidget::FactionHeadSpaceshipLabel, "The \"Bheet\" Spaceship"s}
            }
        };
}

#endif //RTYPE_CREATECONFIG_HPP
