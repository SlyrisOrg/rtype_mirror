//
// Created by milerius on 06/11/17.
//

#include <rtype/config/ConfigurationBase.hpp>
#include <rtype/config/GameConfig.hpp>

namespace cfg::game
{
    //! The two next variables below canno't be inlined, thanks to MSVC bug.
    unsigned int width = cfg::maxWidth;
    unsigned int height = cfg::maxHeight;
    inline std::string lang = cfg::defaultLang;
    inline std::string apiURL = "http://rtype.slyris.eu/api/";
    inline std::string tokenSession = "INVALID TOKEN";
}
