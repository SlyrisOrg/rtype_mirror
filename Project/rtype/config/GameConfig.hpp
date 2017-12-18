//
// Created by milerius on 06/11/17.
//

#ifndef RTYPE_GAMECONFIG_HPP
#define RTYPE_GAMECONFIG_HPP

#include <string>
#include <SFML/Window/WindowStyle.hpp>

namespace cfg::game
{
    //! Game Settings
    extern unsigned int width;
    extern unsigned int height;
    extern bool soundseffect;
    extern bool music;
    extern bool fullscreen;
    extern float soundseffectVolume;
    extern float musicVolume;
    extern std::string lang;

    //! API
    extern std::string apiURL;
    extern std::string tokenSession;
}

#endif //RTYPE_GAMECONFIG_HPP
