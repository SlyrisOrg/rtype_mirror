//
// Created by szterg_r on 29/10/2017.
//

#ifndef RTYPE_LOGINCONFIG_HPP
#define RTYPE_LOGINCONFIG_HPP

#include <string>
#include <CEGUI/Size.h>

namespace cfg::login
{
    extern std::string loginLayout;
    extern std::string loginButtonText;
    extern std::string exitButtonText;
    extern std::string exitYesButtonText;
    extern std::string exitNoButtonText;
    extern std::string exitMessageText;
    extern std::string errorYesText;
    extern std::string accountInformationImage;
    extern std::string accountNameLabelImage;
    extern std::string accountPasswordLabelImage;
    extern CEGUI::USize accountInformationSize;
    extern CEGUI::USize accountNameLabelSize;
    extern CEGUI::USize accountPasswordLabelSize;
}

#endif //RTYPE_LOGINCONFIG_HPP
