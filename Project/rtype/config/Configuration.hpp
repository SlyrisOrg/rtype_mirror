//
// Created by roman sztergbaum on 16/09/2017.
//

#ifndef RTYPE_CONFIGURATION_HPP
#define RTYPE_CONFIGURATION_HPP

/**
 * @brief
 *
 * Please, for the users of Visual Studio and Resharper please do not remove the <unused include files>,
 * it creates inter dependencies of headers, and it is the beginning of a long nightmare.
 *
 * @author Roman Sztergbaum
 */

#include <string>
#include <rapidjson/document.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <sfeMovie/Movie.hpp>
#include <config/Config.hpp>
#include <utils/Enums.hpp>
#include <utils/StringHelpers.hpp>
#include <log/Logger.hpp>
#include <rtype/utils/ResourceManager.hpp>
#include <rtype/utils/Animation.hpp>
#include <rtype/config/ConfigurationMacros.hpp>
#include <rtype/config/ConfigurationBase.hpp>
#include <rtype/config/LoginConfig.hpp>
#include <rtype/config/ErrorConfig.hpp>
#include <rtype/config/CreateConfig.hpp>
#include <rtype/config/GameConfig.hpp>

namespace rtype
{
    namespace lg = logging;
    /**
     * @class Configuration
     * @brief This class is used to configure the game and his environnement
     */
    class Configuration
    {
    public:
        /**
         * @note Each one of the resource types is limited to 125 items, due to the limitation of msvc macros.
         * If you want to have more than 125 resources, you need to create a new type, and its resource manager.
         */
        
        //! Sprite Enum
        RESOURCE_ADD(Sprite,
                     BheetLv1, // 0
                     KooyLv1, // 1
                     MaulLv1, // 2
                     ShipInterior);
        
        //! Video Enum
        RESOURCE_ADD(Video, Intro);
        
        //! Music Enum
        RESOURCE_ADD(Music, BattleExtInstrumentalAmb, AbandonedFacilitiesInstrumentalAmb, SpaceshipBridgeInstrumentalAmb);
        
        //! SoundEffect Enum
        RESOURCE_ADD(SoundEffect, Click, ClickLogin, LoginSuccess, ComputerError);
        
        //! Animation Enum
        RESOURCE_ADD(Animation, BheetLv1, KooyLv1, MaulLv1);
    public:
        /**
         * @fn static void initialize()
         * @brief
         * This function is called before the start of the game,
         * and initializes the main resources that will be needed during the game.
         * The function is static because the Configuration class is not instantiable.
         * @see Configuration constructor
         */
        static void initialize();

    private:
        using TAnimation = Animation::EnumType;
        using TSprite = Sprite::EnumType;
        using TVideo = Video::EnumType;
        using TMusic = Music::EnumType;
        using TSndEffect = SoundEffect::EnumType;
    private:
        //! Private static functions
        //! Used in intern by initialize() public static function

        //! Private init functions

        /**
         * @fn static void __initConfig()
         * @note
         * This function is the first called by initialize (), it parses the program's JSON configuration files.
         */
        static void __initConfig() noexcept;

        /**
         * @fn static void __initTextures()
         * @note
         * This function is called by initialize(), it loads the SFML textures.
         * @throw std::runtime_error if a texture cannot be loaded.
         */
        static void __initTextures();

        /**
         * @fn static void __initMusics()
         * @note
         * This function is called by initialize(), it loads the SFML musics.
         * @throw std::runtime_error if one music cannot be loaded.
         */
        static void __initMusics();

        /**
         * @fn static void __initVideos()
         * @note
         * This function is called by initialize(), it loads the sfe::Movie videos.
         * @throw std::runtime_error if one video cannot be loaded.
         */
        static void __initVideos();

        /**
         * @fn static void __initSoundEffects()
         * @note
         * This function is called by initialize(), it loads the SFML soundseffects.
         * @throw std::runtime_error if a sound effect cannot be loaded.
         */
        static void __initSoundEffects();

        /**
         * @fn static void __initAnims()
         * @note
         * This functions is called by initialize(), it loads the SFML animations.
         * @throw std::runtime_error if an animation cannot be loaded
         */
        static void __initAnims();

        /**
         * @fn static void __initFrames(TAnimation animation, TSprite sprite, int x, int y)
         * @param animation anim enum values to be initialized.
         * @param sprite sprite sheet of the animation to be itinialized.
         * @param x The number of squares of the sprite sheet per line.
         * @param y The number of squares of the sprite sheet per column
         * @note
         * This functions is called by __initAnims(), it loads the frames of an animation.
         */
        static void __initFrames(TAnimation animation, TSprite sprite, int x, int y) noexcept;

        //! Private get functions

        /**
         * @fn static void __getLangGameConfig(const rapidjson::Document &document)
         * @param document The json file that represents game configuration.
         * @note
         * This function is called by __initConfig(), it loads the language from the json document,
         * otherwise he put the default language used by the game to english.
         * @attention rapidjson assert if the field of the Lang doesn't exist.
         */
        static void __getLangGameConfig(const rapidjson::Document &document) noexcept;

        /**
         * @fn static void __getWindowSize(const rapidjson::Document &document)
         * @param document The json file that represents the game configuration.
         * @note
         * This function is called by __initConfig(), it load the window properties from the json,
         * otherwise he put the default properties to the max size.
         * This function is noexcept
         * @attention rapidjson assert if the field of the Size doesn't exist.
         */
        static void __getWindowSize(const rapidjson::Document &document) noexcept;

        //! Private parsing functions

        /**
         * @fn static void __prepareParse(const std::string& path, rapidjson::Document &document)
         * @param path The path to the Json configuration file.
         * @param document The document that will represent the JSON and be parsed.
         * @note
         * This function will open the JSON file
         * found in the path sent as a parameter and store it in the Document.
         * Then the document will be parsed.
         * This function is used by __parse*Config() and __initConfig() functions.
         * @throw std::runtime_error if the document have errors during the parsing, catch internally.
         */
        static void __prepareParse(const std::string &path, rapidjson::Document &document);

        /**
         * @fn static void __parseLangConfig()
         * @note
         * This function parse each scenes Configuration files for the game language.
         * @throw see __prepareParse
         */
        static void __parseLangConfig();

        /**
         * @fn static void __parseLoginConfig()
         * @note
         * This function parse the JSON file that contains the Login scene configurations.
         * @throw from __prepareParse
         */
        static void __parseLoginConfig();

        /**
         * @fn static void __parseCreateConfig()
         * @note
         * This function parse the JSON file that contains the Create scene configurations.
         * @throw from __prepareParse
         */
        static void __parseCreateConfig();

        /**
         * @fn static void __parseLoginConfig()
         * @note
         * This function parse the JSON file that contains the Error code configurations.
         * @throw from __prepareParse
         */
        static void __parseErrorConfig();

    public:
        /**
         * @var textures
         * @note
         * This variable is both public and static to get access from anywhere in the program.
         * It allows to manipulate the SFML textures that were load by the initialize() function.
         */
        static sfutils::ResourceManager<sf::Texture, TSprite> textures;

        /**
         * @var animations
         * @note
         * This variable is both public and static to allow access from anywhere in the program.
         * It allows manipulation of the SFML animations loaded by the initialize() function.
         */
        static sfutils::ResourceManager<sfutils::Animation, TAnimation> animations;

        /**
        * @var musics
        * @note
        * This variable is both public and static to get access from anywhere in the program.
        * It allows manipulation of the SFML musics loaded by the initialize() function.
        */
        static sfutils::ResourceManager<sf::Music, TMusic> musics;

        /**
        * @var videos
        * @note
        * This variable is both public and static to get access from anywhere in the program.
        * It allows manipulation of sfe::Movie videos loaded by the initialize() function.
        */
        static sfutils::ResourceManager<sfe::Movie, TVideo> videos;

        /**
        * @var effects
        * @note
        * This variable is both public and static to get access from anywhere in the program.
        * It allows manipulation of SFML soundseffects loade by the initialize() function.
        */
        static sfutils::ResourceManager<sf::SoundBuffer, TSndEffect> effects;
    private:
        /**
         * @var log
         * @note
         * This variable is used for internal logging of Configuration class.
         * This variable is also static because the Configuration class is not instantiable.
         */
        static lg::Logger log;

    private:
        //! Configuration class is not instantiable.
        Configuration() = default;
    };
}

#endif //RTYPE_CONFIGURATION_HPP
