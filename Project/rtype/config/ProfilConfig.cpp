//
// Created by milerius on 25/11/17.
//

#include <rtype/config/ProfilConfig.hpp>

namespace cfg::profil
{
    //! Profil informations.
    inline std::string level;
    FactionT faction;
    inline std::string gold;
    inline std::string goldShop;
    inline std::string nickname;

    //! Lang
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