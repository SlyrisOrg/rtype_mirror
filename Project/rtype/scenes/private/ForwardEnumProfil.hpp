//
// Created by milerius on 12/12/17.
//

#ifndef RTYPE_FORWARDENUMPROFIL_HPP
#define RTYPE_FORWARDENUMPROFIL_HPP

#include <utils/Enums.hpp>

namespace rtype
{
    ENUM(ProfilWidgets,
         Bheet,
         Kooy,
         Maul,
         LevelLabel,
         GoldNormalMoney,
         GoldShopMoney,
         Pseudo,
         XPBar,
         BackButton,
         ExitDesktopButton,
         LogOutButton,
         MainMenuHead,
         MainMenuLabel,
         OptionsButton,
         MainMenu,
         OptionsWindow,
         OptionsCloseButton,
         OptionsHead,
         OptionsRealHead,
         ApplyButton,
         ShopButton,
         ProfileButton,
         PlayButton,
         TabButtonLang,
         TabButtonSounds,
         RightPanelLangSettings,
         RightPanelSoundsSettings,
         MusicCheckBox,
         SoundsEffectCheckBox,
         MusicSlider,
         SoundsEffectSlider,
         LabelMusic,
         LabelSoundsEffect,
         SoundsEffectSliderProgress,
         MusicSliderProgress,
         fr,
         en,
         DiscardButton,
         TabButtonScreen,
         ModeWindow,
         ModeOptionsHead,
         ModeOptionsRealHead,
         ModeOptionsCloseButton,
         ModeLocalCheck,
         ModeTwoCheck,
         ModeThreeCheck,
         ModeFourCheck,
         ModeOkButton
    );

    using TProfilWidget = ProfilWidgets::EnumType;
}

#endif //RTYPE_FORWARDENUMPROFIL_HPP
