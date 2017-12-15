//
// Created by szterg_r on 29/10/2017.
//

#include <rtype/config/CreateConfig.hpp>

namespace cfg::create
{
    inline std::string chooseFactionImage = "RTFaction/TextCreate";
    inline CEGUI::USize chooseFactionSize = {{0.f, 192.f}, {0.f, 28.f}};
    inline std::unordered_map<std::string, rtype::AnimInfo> animCreate;
    inline std::unordered_map<std::string, std::vector<sfutils::AnimatedSprite>> multiAnimMap;
}