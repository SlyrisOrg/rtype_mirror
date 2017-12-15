//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/LoginConfig.hpp>

namespace cfg::login
{
    using namespace std::string_literals;
    inline std::string loginLayout = "RTLogin.layout"s;
    inline std::string accountInformationImage = "RTAccount/Information"s;
    inline std::string accountNameLabelImage = "RTAccountName/AccountName"s;
    inline std::string accountPasswordLabelImage = "RTPassword/Password"s;
    inline CEGUI::USize accountInformationSize = {{0.f, 207.f}, {0.f,11.f}};
    inline CEGUI::USize accountNameLabelSize = {{0.f, 102.f}, {0.f,10.f}};
    inline CEGUI::USize accountPasswordLabelSize = {{0.f, 117.f}, {0.f,10.f}};
}