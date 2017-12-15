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
#include <atomic>

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
                     BheetArrival,
                     KooyArrival,
                     MaulArrival,
                     ShipInterior,
                     BGProfil);

        //! Video Enum
        RESOURCE_ADD(Video, Intro);

        //! Music Enum
        RESOURCE_ADD(Music,
                     BattleExtInstrumentalAmb,
                     AbandonedFacilitiesInstrumentalAmb,
                     SpaceshipBridgeInstrumentalAmb,
                     MarketInstrumentalAmb);

        //! SoundEffect Enum
        RESOURCE_ADD(SoundEffect,
                     Click,
                     ClickSmooth,
                     LoginSuccess,
                     ComputerError,
                     ChangeFaction);

        //! Animation Enum
        RESOURCE_ADD(Animation,
                     BheetLv1,
                     KooyLv1,
                     MaulLv1,
                     BheetArrival,
                     KooyArrival,
                     MaulArrival);
    public:
        /**
         * @brief
         * This function is called before the start of the game,
         * and initializes the main resources that will be needed during the game.
         * The function is static because the Configuration class is not instantiable.
         * @see Configuration constructor
         */
        static void initialize(std::atomic_uint &nbCounts);

        static void changeLang(const std::string &lang) noexcept;
    public:
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
         * @note
         * This function is the first called by initialize (), it parses the program's JSON configuration files.
         */
        static void __initConfig(std::atomic_uint &currentNbFiles) noexcept;

        /**
         * @note
         * This function is called by initialize(), it loads the SFML textures.
         * @throw std::runtime_error if a texture cannot be loaded.
         */
        static void __initTextures(std::atomic_uint &nbCounts);

        /**
         * @note
         * This function is called by initialize(), it loads the SFML musics.
         * @throw std::runtime_error if one music cannot be loaded.
         */
        static void __initMusics(std::atomic_uint &atomic);

        /**
         * @fn static void __initVideos()
         * This function is called by initialize(), it loads the sfe::Movie videos.
         * @throw std::runtime_error if one video cannot be loaded.
         */
        static void __initVideos(std::atomic_uint &atomic);

        /**
         * @note
         * This function is called by initialize(), it loads the SFML soundseffects.
         * @throw std::runtime_error if a sound effect cannot be loaded.
         */
        static void __initSoundEffects(std::atomic_uint &atomic);

        /**
         * @note
         * This functions is called by initialize(), it loads the SFML animations.
         * @throw std::runtime_error if an animation cannot be loaded
         */
        static void __initAnims(std::atomic_uint &atomic);

    private:
        /**
         * @param animation anim enum values to be initialized.
         * @param sprite sprite sheet of the animation to be itinialized.
         * @param nbColumns The number of squares of the sprite sheet per line.
         * @param nbLines The number of squares of the sprite sheet per column
         * @note
         * This functions is called by __initAnims(), it loads the frames of an animation.
         */
        static void __initFrames(TAnimation animation, TSprite sprite, unsigned int nbColumns, unsigned int nbLines,
                                 unsigned int nbAnims, std::atomic_uint &nbCounts) noexcept;

        //! Private get functions

        /**
         * @param document The json file that represents game configuration.
         * @note
         * This function is called by __initConfig(), it loads the language from the json document,
         * otherwise he put the default language used by the game to english.
         * @attention rapidjson assert if the field of the Lang doesn't exist.
         */
        static void __getLangGameConfig(const rapidjson::Document &document) noexcept;

        /**
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
         * @param path The path to the Json configuration file.
         * @param document The document that will represent the JSON and be parsed.
         * @note
         * This function will open the JSON file
         * found in the path sent as a parameter and store it in the Document.
         * Then the document will be parsed.
         * This function is used by __parse*Config() and __initConfig() functions.
         * @throw std::runtime_error if the document have errors during the parsing, catch internally.
         */
        static void __loadJSON(const std::string &path, rapidjson::Document &document);

        static void __parseAnims(std::atomic_uint &atomic);
        static void __parseCreateAnims();
        /**
         * @note
         * This function parse each scenes Configuration files for the game language.
         * @throw see __prepareParse
         */
        static void __parseLangConfig();
        static void __parseLangConfig(std::atomic_uint &nbCounts);
        /**
         * @note
         * This function parse the JSON file that contains the Login scene configurations.
         */
        static void __parseLoginConfig();

        /**
         * @note
         * This function parse the JSON file that contains the Create scene configurations.
         * @throw from __prepareParse
         */
        static void __parseCreateConfig();

        /**
         * @note
         * This function parse the JSON file that contains the Error code configurations.
         * @throw from __prepareParse
         */
        static void __parseErrorConfig();

        /**
         * @note
         * This function parse the JSON file that contains the Profil scene configurations.
         * @throw from __prepareParse
         */
        static void __parseProfilConfig();

        template <typename Map>
        static void __fillAnimationsInfos(const rapidjson::Value &value, Map &map);

        template <typename Map, typename AnimMap>
        static void __fillMultiAnim(const rapidjson::Value &value, Map &map, AnimMap &animMap);

        template <typename EnumT, typename Handle>
        static void __initResources(const std::string &path, const std::string &suffix, Handle &handle,
                                    std::string_view resourceType, std::atomic_uint &nbCounts)
        {
            for (const auto &cur : EnumT::values()) {
                auto str = path + cur.toString() + suffix;
                handle.load(cur, str);
                log(lg::Debug) << "Loading " << resourceType << " " << cur.toString() + suffix << " successfully loaded."
                               << std::endl;
                nbCounts++;
            }
        }

    public:

        static sfutils::ResourceManager<sf::Texture, TSprite> textures;
        static sfutils::ResourceManager<sfutils::Animation, TAnimation> animations;
        static sfutils::ResourceManager<sf::Music, TMusic> musics;
        static sfutils::ResourceManager<sfe::Movie, TVideo> videos;
        static sfutils::ResourceManager<sf::SoundBuffer, TSndEffect> effects;

    private:
        static lg::Logger log;

    private:
        //! Configuration class is not instantiable.
        Configuration() = default;
    };
}

#endif //RTYPE_CONFIGURATION_HPP
