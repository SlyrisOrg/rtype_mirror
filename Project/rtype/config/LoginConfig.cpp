//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/LoginConfig.hpp>

namespace cfg::login
{
    inline std::string loginLayout = "RTLogin.layout";
    inline std::string loginButtonText = "LOGIN";
    inline std::string exitButtonText = "EXIT";
    inline std::string exitYesButtonText = "YES";
    inline std::string exitNoButtonText = "NO";
    inline std::string exitMessageText = "Do you want to quit the game ?";
    inline std::string errorYesText = "OK";
    inline std::string accountInformationImage = "RTAccount/Information";
    inline std::string accountNameLabelImage = "RTAccountName/AccountName";
    inline std::string accountPasswordLabelImage = "RTPassword/Password";
    inline CEGUI::USize accountInformationSize = {{0.f, 207.f}, {0.f,11.f}};
    inline CEGUI::USize accountNameLabelSize = {{0.f, 102.f}, {0.f,10.f}};
    inline CEGUI::USize accountPasswordLabelSize = {{0.f, 117.f}, {0.f,10.f}};
}