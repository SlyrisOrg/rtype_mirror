//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/CreateConfig.hpp>

namespace cfg::create
{
    //! Layout
    inline std::string chooseFactionImage = "RTFaction/TextCreate";

    //! Property
    inline CEGUI::USize chooseFactionSize = {{0.f, 192.f}, {0.f, 28.f}};

    //! Animations
    inline std::unordered_map<std::string, rtype::AnimInfo> animCreate;
    inline std::unordered_map<std::string, std::vector<sfutils::AnimatedSprite>> multiAnimMap;

    //! Lang
    inline std::unordered_map<std::string, CLabelValues> createLabel
        {
            {
                "backbutton", {TCreateWidget::BackButton, "BACK"}
            },
            {
                "createbutton", {TCreateWidget::CreateButton, "CREATE"}
            },
            {
                "factionheadlabel", {TCreateWidget::FactionHeadLabel, "The \"Bheet\" faction"}
            },
            {
                "factionheadspaceshiplabel", {TCreateWidget::FactionHeadSpaceshipLabel, "The \"Bheet\" Spaceship"}
            }
        };
}