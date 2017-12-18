//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/LoginConfig.hpp>

namespace cfg::login
{
    //! Layout
    inline std::string loginLayout = "RTLogin.layout";

    //! Property
    inline std::string accountInformationImage = "RTAccount/Information";
    inline std::string accountNameLabelImage = "RTAccountName/AccountName";
    inline std::string accountPasswordLabelImage = "RTPassword/Password";
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
                "logbutton", {TLoginWidget::LogButton, "LOGIN"}
            },
            {
                "exitbutton", {TLoginWidget::ExitButton, "EXIT"}
            },
            {
                "exityesbutton", {TLoginWidget::YesQuit, "YES"}
            },
            {
                "exitnobutton", {TLoginWidget::NoQuit, "NO"}
            },
            {
                "exittextmessage", {TLoginWidget::ExitText, "Do you want to quit the game ?"}
            },
            {
                "erroryesbutton", {TLoginWidget::ErrorYesQuit, "OK"}
            },
        };
}