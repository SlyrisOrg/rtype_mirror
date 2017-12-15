//
// Created by milerius on 25/11/17.
//

#ifndef RTYPE_PROFILCONFIG_HPP
#define RTYPE_PROFILCONFIG_HPP

#include <string>
#include <unordered_map>
#include <utils/Enums.hpp>
#include <rtype/scenes/private/ForwardEnumProfil.hpp>

namespace cfg::profil
{
    ENUM(Faction,
         Bheet, // 0
         Kooy,  // 1
         Maul) // 2

    using FactionT = Faction::EnumType;

    static inline constexpr const char *profilLayout = "RTMainScene.layout";
    static inline constexpr unsigned int nbWidgets = 25;

    //! Others
    extern std::string level;
    extern FactionT faction;
    extern std::string gold;
    extern std::string goldShop;
    extern std::string nickname;

    //! Character experience
    inline float xp = 0.f;


    /**
     * @note string label, for langage.
     */
    using TProfilWidget = rtype::PW::EnumType;
    using LabelValues = std::pair<TProfilWidget, std::string>;
    using namespace std::string_literals;
    inline std::unordered_map<std::string, LabelValues> profileLabel
        {
            {
                "levellabel"s, {TProfilWidget::LevelLabel, "LEVEL"s}
            },
            {
                "playlabel"s, {TProfilWidget::PlayButton, "PLAY"s}
            },
            {
                "profilelabel"s, {TProfilWidget::ProfileButton, "        PROFILE"s}
            },
            {
                "shoplabel"s, {TProfilWidget::ShopButton, "        SHOP"s}
            },
            {
                "optionsbuttonlabel"s, {TProfilWidget::OptionsButton, "OPTIONS"s}
            },
            {
                "logoutbuttonlabel"s, {TProfilWidget::LogOutButton, "LOG OUT"s}
            },
            {
                "exitdesktopbuttonlabel"s, {TProfilWidget::ExitDesktopButton, "EXIT TO DESKTOP"s}
            }
        };
}

#endif //RTYPE_PROFILCONFIG_HPP
