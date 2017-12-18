//
// Created by milerius on 06/11/17.
//

#ifndef RTYPE_CONFIGURATIONBASE_HPP
#define RTYPE_CONFIGURATIONBASE_HPP

/**
 * @attention
 *
 * This comment is a general comment on the configuration of RType.
 * As you can see the files are not header only as usual on other projects.
 * All simply because in the programming of video games we prefer the readability,
 * the ability to maintain some of the code.
 * Apart from the functions subject to variadic templates, and some short functions that might be inline,
 * The rest is not header only.
 * The configuration is complete for the moment but improveable in the near future,
 * it is currently almost exception safety but it is expected to have an additional overload for the resource manager
 * to be completely exception safety but this concerns another part of the code (utils).
 *
 * @details in the configuration of RType we are using one of the last features of the c++17 : Inline variables.
 *  An inline variable (since C++17) with external linkage has the following additional properties:
 *  It must be declared inline in every translation unit.
 *  It has the same address in every translation unit.
 *  An inline variable may be defined in a header file that is #include'd in multiple source files.
 * @source http://en.cppreference.com/w/cpp/language/inline
 * @external https://www.youtube.com/watch?v=xVT1y0xWgww
 * this talk speak about global variables in general an interest of using inline variables in header file and
 * inline variable for extern variables in c++ source files.
 * @author Roman Sztergbaum
 */

namespace cfg
{
    inline static constexpr const unsigned int maxWidth = 1920;
    inline static constexpr const unsigned int maxHeight = 1080;
    inline static constexpr const unsigned int nbConfigLang = 4;
    inline static constexpr const unsigned int nbConfigAnimations = 1;
    inline static constexpr const unsigned int nbConfigs = 1 + nbConfigLang + nbConfigAnimations;
    inline static constexpr const char *defaultLang = "en";
    inline static constexpr const char *title = "RType";
    inline static constexpr const char *animConfigPath = "assets/config/animations/";
    inline static constexpr const char *configPath = "assets/config/";
    inline static constexpr const char *guiRoot = "assets/gui/";
    inline static constexpr const char *spritePath = "assets/sprite/";
    inline static constexpr const char *videoPath = "assets/video/";
    inline static constexpr const char *soundEffectPath = "assets/soundeffects/";
    inline static constexpr const char *musicPath = "assets/music/";
}

#endif //RTYPE_CONFIGURATIONBASE_HPP
