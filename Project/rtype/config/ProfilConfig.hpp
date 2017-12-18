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
    //! Infos
    ENUM(Faction,
         Bheet, // 0
         Kooy,  // 1
         Maul) // 2

    using FactionT = Faction::EnumType;

    //! Layouts
    static inline constexpr const char *profilLayout = "RTMainScene.layout";
    static inline constexpr unsigned int nbWidgets = 37;

    //! Profil informations.
    extern std::string level;
    extern FactionT faction;
    extern std::string gold;
    extern std::string goldShop;
    extern std::string nickname;
    inline float xp = 0.f;

    //! Lang
    using TProfilWidget = rtype::ProfilWidgets::EnumType;
    using LabelValues = std::pair<TProfilWidget, std::string>;
    using namespace std::string_literals;
    extern std::unordered_map<std::string, LabelValues> profileLabel;
}

#endif //RTYPE_PROFILCONFIG_HPP
