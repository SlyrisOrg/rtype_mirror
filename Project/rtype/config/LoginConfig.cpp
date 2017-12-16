//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/LoginConfig.hpp>

namespace cfg::login
{
    using namespace std::string_literals;

    //! Layout
    inline std::string loginLayout = "RTLogin.layout"s;

    //! Property
    inline std::string accountInformationImage = "RTAccount/Information"s;
    inline std::string accountNameLabelImage = "RTAccountName/AccountName"s;
    inline std::string accountPasswordLabelImage = "RTPassword/Password"s;
    inline CEGUI::USize accountInformationSize = {{0.f, 207.f},
                                                  {0.f, 11.f}};
    inline CEGUI::USize accountNameLabelSize = {{0.f, 102.f},
                                                {0.f, 10.f}};
    inline CEGUI::USize accountPasswordLabelSize = {{0.f, 117.f},
                                                    {0.f, 10.f}};

    //! Lang
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