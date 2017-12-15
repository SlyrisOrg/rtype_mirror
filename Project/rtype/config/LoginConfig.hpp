//
// Created by szterg_r on 29/10/2017.
//

#ifndef RTYPE_LOGINCONFIG_HPP
#define RTYPE_LOGINCONFIG_HPP

#include <unordered_map>
#include <string>
#include <CEGUI/Size.h>
#include <rtype/scenes/private/ForwardEnumLogin.hpp>

namespace cfg::login
{
    //! Layouts
    extern std::string loginLayout;

    //! Property
    extern std::string accountInformationImage;
    extern std::string accountNameLabelImage;
    extern std::string accountPasswordLabelImage;
    extern CEGUI::USize accountInformationSize;
    extern CEGUI::USize accountNameLabelSize;
    extern CEGUI::USize accountPasswordLabelSize;

    //! Lang
    using namespace std::string_literals;
    using TLoginWidget = rtype::LOGW::EnumType;
    using TLabelValues = std::pair<TLoginWidget, std::string>;
    inline std::unordered_map<std::string, TLabelValues> logLabel
        {
            {
                "logbutton"s, {TLoginWidget::LogButton, "LOGIN"s}
            },
            {
                "exitbutton"s, {TLoginWidget::ExitButton, "EXIT"s}
            },
            {
                "exityesbutton"s, {TLoginWidget::YesQuit, "YES"s}
            },
            {
                "exitnobutton"s, {TLoginWidget::NoQuit, "NO"s}
            },
            {
                "exittextmessage"s, {TLoginWidget::ExitText, "Do you want to quit the game ?"s}
            },
            {
                "erroryesbutton"s, {TLoginWidget::ErrorYesQuit, "OK"s}
            },
        };
}

#endif //RTYPE_LOGINCONFIG_HPP
