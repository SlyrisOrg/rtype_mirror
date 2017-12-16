//
// Created by milerius on 06/11/17.
//

#include <rtype/config/ConfigurationBase.hpp>
#include <rtype/config/GameConfig.hpp>

namespace cfg::game
{
    using namespace std::string_literals;
    unsigned int width = cfg::maxWidth;
    unsigned int height = cfg::maxHeight;
    inline std::string lang = cfg::defaultLang;
    inline std::string apiURL = "http://rtype.slyris.eu/api/"s;
    inline std::string tokenSession = "JWT "s;
}
