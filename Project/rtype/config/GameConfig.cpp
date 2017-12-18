//
// Created by milerius on 06/11/17.
//

#include <rtype/config/ConfigurationBase.hpp>
#include <rtype/config/GameConfig.hpp>

namespace cfg::game
{
    bool soundseffect = true;
    bool music = true;
    bool fullscreen = false;
    float soundseffectVolume = 70.0f;
    float musicVolume = 30.0f;
    unsigned int width = cfg::maxWidth;
    unsigned int height = cfg::maxHeight;
    inline std::string lang = cfg::defaultLang;
    inline std::string apiURL = "http://rtype.slyris.eu/api/";
    inline std::string tokenSession = "JWT ";
}
