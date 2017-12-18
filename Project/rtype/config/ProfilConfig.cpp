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
                "levellabel", {TProfilWidget::LevelLabel, "LEVEL"}
            },
            {
                "playlabel", {TProfilWidget::PlayButton, "PLAY"}
            },
            {
                "profilelabel", {TProfilWidget::ProfileButton, "        PROFILE"}
            },
            {
                "shoplabel", {TProfilWidget::ShopButton, "        SHOP"}
            },
            {
                "optionsbuttonlabel", {TProfilWidget::OptionsButton, "OPTIONS"}
            },
            {
                "logoutbuttonlabel", {TProfilWidget::LogOutButton, "LOG OUT"}
            },
            {
                "exitdesktopbuttonlabel", {TProfilWidget::ExitDesktopButton, "EXIT TO DESKTOP"}
            },
            {
                "soundslabel", {TProfilWidget::LabelSoundsEffect, "Sounds Effect Volume: "}
            },
            {
                "musiclabel", {TProfilWidget::LabelMusic, "Music Volume: "}
            }

        };
}