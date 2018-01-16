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

#include <atomic>
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
         * @details Each one of the resource types is limited to 125 items, due to the limitation of msvc macros.
         * If you want to have more than 125 resources, you need to create a new type, and its resource manager.
         */

        //! Sprite Enum
        ENUM(Sprite,
             BheetLv1, // 0
             KooyLv1, // 1
             MaulLv1, // 2
             BheetArrival,
             KooyArrival,
             MaulArrival,
             ShipInterior,
             BGProfil,
             QueueBackground,
             BorderBG);

        //! Video Enum
        ENUM(Video,
             Intro);

        //! Music Enum
        ENUM(Music,
             BattleExtInstrumentalAmb,
             AbandonedFacilitiesInstrumentalAmb,
             SpaceshipBridgeInstrumentalAmb,
             MarketInstrumentalAmb,
             DarkVoidAmb,
             CitadelInstrumentalAmb);

        //! SoundEffect Enum
        ENUM(SoundEffect,
             Click,
             ClickSmooth,
             LoginSuccess,
             ComputerError,
             ChangeFaction,
             Laser4);

        //! Animation Enum
        ENUM(Animation,
             BheetLv1,
             KooyLv1,
             MaulLv1,
             BheetArrival,
             KooyArrival,
             MaulArrival);
    public:
        /**
         * @brief
         * Called before the start of the game, and initializes the main resources that will be needed during the game.
         */
        static void initialize(std::atomic_uint &nbCounts);

        /**
         * @brief changing the langage at runtime.
         */
        static void changeLang(const std::string &lang) noexcept;

        /**
         * @brief changing music settings at runtime.
         */
        static void changeMusic(float musicVolume) noexcept;

        /**
         * @brief changing sounds effect settings at runtime
         */
        static void changeSoundsEffect(float soundsEffectVolume) noexcept;

        /**
         * @brief used before the launching of rtype for screen settings.
         */
        static void contextSettings() noexcept;

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
         * @brief Generic function for load resources.
         *
         * @tparam EnumT    type of the resource to load
         * @tparam Handle   type of the resource container
         *
         * @param path      path to the resource folder to load
         * @param ext       file extension of the resource to load (for example: avi, png, mp3, ...)
         */
        template <typename EnumT, typename Handle>
        static void __initResources(const std::string &path, const std::string &ext, Handle &handle,
                                    std::string_view resourceType, std::atomic_uint &nbCounts)
        {
            for (const auto &cur : EnumT::values()) {
                auto str = path + cur.toString() + ext;
                handle.load(cur, str);
                _log(lg::Debug) << "Loading " << resourceType << " " << cur.toString() + ext
                                << " successfully loaded."
                                << std::endl;
                nbCounts++;
            }
        }

        /**
         * @group Resources loading/initializing functions
         */
        static void __initConfig(std::atomic_uint &currentNbFiles) noexcept;
        static void __initTextures(std::atomic_uint &nbCounts);
        static void __initMusics(std::atomic_uint &atomic);
        static void __initVideos(std::atomic_uint &atomic);
        static void __initSoundEffects(std::atomic_uint &atomic);
        static void __initAnims(std::atomic_uint &atomic);

        /**
         * @brief Load the frames of an animation.
         *
         * @param animation anim enum values to be initialized.
         * @param sprite    sprite sheet of the animation to be itinialized.
         * @param nbColumns the number of squares of the sprite sheet per line.
         * @param nbLines   the number of squares of the sprite sheet per column
         */
        static void __initFrames(TAnimation animation, TSprite sprite, unsigned int nbColumns, unsigned int nbLines,
                                 unsigned int nbAnims, std::atomic_uint &nbCounts) noexcept;

        //! Private get functions

        /**
         * @details
         * This function loads the language from the json document,
         * @param document The json file that represents game configuration.
         * otherwise he put the default language used by the game to english.
         * @attention rapidjson assert if the field of the Lang doesn't exist.
         */
        static void __getLangGameConfig(const rapidjson::Document &document) noexcept;
        static void __getSoundsConfig(const rapidjson::Document &document) noexcept;

        /**
         * @param document The json file that represents the game configuration.
         * @details
         * This function is called by __initConfig(), it load the window properties from the json,
         * otherwise he put the default properties to the max size.
         * @attention rapidjson assert if the field of the Size doesn't exist.
         */
        static void __getWindowSize(const rapidjson::Document &document) noexcept;

        //! Private parsing functions

        /**
         * @brief Parse the given JSON file and store it as a document
         *
         * @param path      the path to the JSON configuration file.
         * @param document  the document in which to store the result.
         */
        static void __loadJSON(const std::string &path, rapidjson::Document &document);

        /**
         * @group Configuration parsing functions
         */
        static void __parseAnims(std::atomic_uint &atomic);
        static void __parseCreateAnims();
        static void __parseLangConfig();
        static void __parseLangConfig(std::atomic_uint &nbCounts);
        static void __parseLoginConfig();
        static void __parseCreateConfig();
        static void __parseErrorConfig();
        static void __parseProfilConfig();

        /**
         * @brief Fill a container of animations from a JSON value
         *
         * @tparam Map      the type of container to fill
         *
         * @param animValue     the JSON value to extract from
         */
        template <typename Map>
        static void __fillAnimationsInfos(const rapidjson::Value &animValue, Map &map);

        /**
         * @brief Fill a container of sequences of animations from a JSON value
         *
         * @tparam Map      the type of container to fill
         * @param value     the JSON value to extract from
         */
        template <typename Map, typename AnimMap>
        static void __fillMultiAnim(const rapidjson::Value &value, Map &map, AnimMap &animMap);

        /**
         * @brief write a json document in a given path
         */
        static void __writeDoc(const std::string &path, rapidjson::Document &doc);

        /**
         * @brief common function for changing music/sounds settings.
         */
        static void __changeSounds(const std::string &label, float volume, const std::string &labelToCheck);

    public:
        static sfutils::ResourceManager<sf::Texture, TSprite> textures;
        static sfutils::ResourceManager<sfutils::Animation, TAnimation> animations;
        static sfutils::ResourceManager<sf::Music, TMusic> musics;
        static sfutils::ResourceManager<sfe::Movie, TVideo> videos;
        static sfutils::ResourceManager<sf::SoundBuffer, TSndEffect> effects;

    private:
        static lg::Logger _log;

    private:
        //! Configuration class is not instantiable.
        Configuration() = default;
    };
}

#endif //RTYPE_CONFIGURATION_HPP
