//
// Created by roman sztergbaum on 16/09/2017.
//

#include <thread>
#include <fstream>
#include <config/Config.hpp>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rtype/config/Configuration.hpp>
#include <rtype/config/ProfilConfig.hpp>
#include <rtype/config/SpaceshipConfig.hpp>

/**
 * @bug Windows standard library have a macro which conflict with rapidjson GetObject.
 */
#ifdef USING_MSVC
#undef GetObject
#endif

//! Static initializations.
namespace rtype
{
    sfutils::ResourceManager<sf::Texture, Configuration::TSprite> Configuration::textures;
    sfutils::ResourceManager<sfutils::Animation, Configuration::TAnimation> Configuration::animations;
    sfutils::ResourceManager<sf::Music, Configuration::TMusic> Configuration::musics;
    sfutils::ResourceManager<sfe::Movie, Configuration::TVideo> Configuration::videos;
    sfutils::ResourceManager<sf::SoundBuffer, Configuration::TSndEffect> Configuration::effects;
    lg::Logger Configuration::_log = lg::Logger{"Configuration", lg::Debug};
}

//! Public functions
namespace rtype
{
    void Configuration::initialize(std::atomic_uint &nbCounts)
    {
        std::thread([&nbCounts]() {
            __initTextures(nbCounts);
            __initAnims(nbCounts);
        }).detach();
        std::thread([&nbCounts]() {
            __initMusics(nbCounts);
            __initSoundEffects(nbCounts);
        }).detach();
        std::thread([&nbCounts]() {
            __initConfig(nbCounts);
            __initVideos(nbCounts);
        }).detach();
    }

    void Configuration::changeLang(const std::string &lang) noexcept
    {
        using namespace std::string_literals;
        try {
            const auto path = cfg::configPath + "config.json"s;
            rapidjson::Document doc;
            __loadJSON(path, doc);
            auto &&gameCFG = doc["gameconfig"];
            gameCFG["lang"].SetString(lang.c_str(), static_cast<rapidjson::SizeType>(lang.length()));
            __writeDoc(path, doc);
            __getLangGameConfig(doc);
            __parseLangConfig();
        }
        catch (const std::runtime_error &err) {
            _log(lg::Error) << err.what() << std::endl;
        }
    }

    void Configuration::changeMusic(float musicVolume) noexcept
    {
        __changeSounds("musicvolume", musicVolume, "music");
    }

    void Configuration::changeSoundsEffect(float soundsEffectVolume) noexcept
    {
        __changeSounds("soundseffectvolume", soundsEffectVolume, "soundseffect");
    }

    void Configuration::contextSettings() noexcept
    {
        using namespace std::string_literals;

        try {
            const auto path = cfg::configPath + "config.json"s;
            rapidjson::Document doc;
            __loadJSON(path, doc);
            __getWindowSize(doc);
        }
        catch (const std::runtime_error &error) {
            _log(lg::Error) << error.what() << std::endl;
        }
    }
}

//! Private functions
namespace rtype
{
    void Configuration::__changeSounds(const std::string &label, float volume, const std::string &labelToCheck)
    {
        using namespace std::string_literals;

        try {
            const auto path = cfg::configPath + "config.json"s;
            rapidjson::Document doc;
            __loadJSON(path, doc);
            auto &&gameCFG = doc["gameconfig"];
            gameCFG[labelToCheck.c_str()].SetBool(volume != 0.0f);
            gameCFG[label.c_str()].SetFloat(volume);
            __writeDoc(path, doc);
            __getSoundsConfig(doc);
        }
        catch (const std::runtime_error &err) {
            _log(lg::Error) << err.what() << std::endl;
        }
    }

    void Configuration::__writeDoc(const std::string &path, rapidjson::Document &doc)
    {
        std::ofstream ofs(path);
        rapidjson::OStreamWrapper osw(ofs);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        doc.Accept(writer);
    }

    void Configuration::__initConfig(std::atomic_uint &currentNbFiles) noexcept
    {
        using namespace std::string_literals;

        try {
            const auto path = cfg::configPath + "config.json"s;
            rapidjson::Document doc;
            __loadJSON(path, doc);
            __getLangGameConfig(doc);
            __getSoundsConfig(doc);
            __getWindowSize(doc);
            currentNbFiles += 1u;
            __parseLangConfig(currentNbFiles);
        }
        catch (const std::runtime_error &error) {
            _log(lg::Error) << error.what() << std::endl;
        }
    }

    void Configuration::__initTextures(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<Sprite>(cfg::spritePath, ".png", textures, "textures"sv, nbCounts);
    }

    void Configuration::__initMusics(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<Music>(cfg::musicPath, ".ogg", musics, "musics"sv, nbCounts);
    }

    void Configuration::__initVideos(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<Video>(cfg::videoPath, ".avi", videos, "video"sv, nbCounts);
    }

    void Configuration::__initSoundEffects(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<SoundEffect>(cfg::soundEffectPath, ".ogg", effects, "sounds effect"sv, nbCounts);
    }

    void Configuration::__getLangGameConfig(const rapidjson::Document &document) noexcept
    {
        auto &&gameCFG = document["gameconfig"];
        cfg::game::lang = gameCFG["lang"].GetString();
        if (cfg::game::lang != "en" && cfg::game::lang != "fr")
            cfg::game::lang = cfg::defaultLang;
        _log(lg::Info) << "The language in game is : " << cfg::game::lang << std::endl;
    }

    void Configuration::__getSoundsConfig(const rapidjson::Document &document) noexcept
    {
        auto &&gameCFG = document["gameconfig"];

        cfg::game::soundseffect = gameCFG["soundseffect"].GetBool();
        cfg::game::music = gameCFG["music"].GetBool();
        cfg::game::musicVolume = gameCFG["musicvolume"].GetFloat();
        cfg::game::soundseffectVolume = gameCFG["soundseffectvolume"].GetFloat();

        if (cfg::game::musicVolume > 100.0f || cfg::game::musicVolume < 0.0f)
            cfg::game::musicVolume = 100.0f;
        if (!cfg::game::music)
            cfg::game::musicVolume = 0.0f;
        if (!cfg::game::soundseffect)
            cfg::game::soundseffectVolume = 0.0f;
        if (cfg::game::soundseffectVolume > 100.0f || cfg::game::soundseffectVolume < 0.0f)
            cfg::game::soundseffectVolume = 100.0f;

        _log(lg::Info) << "\nmusic: " << std::boolalpha << cfg::game::music << "\nsoundseffect: "
                       << cfg::game::soundseffect << std::noboolalpha << "\nmusic volume: " << cfg::game::musicVolume
                       << "\nsounds effect volume: " << cfg::game::soundseffectVolume << std::endl;
    }

    void Configuration::__getWindowSize(const rapidjson::Document &document) noexcept
    {
        auto &&gameCFG = document["gameconfig"];

        cfg::game::width = gameCFG["width"].GetUint();
        cfg::game::height = gameCFG["height"].GetUint();

        if (cfg::game::width > cfg::maxWidth)
            cfg::game::width = cfg::maxWidth;
        if (cfg::game::height > cfg::maxHeight)
            cfg::game::width = cfg::maxHeight;

        cfg::game::fullscreen = gameCFG["fullscreen"].GetBool();
        _log(lg::Info) << "Window size : " << cfg::game::width << ", " << cfg::game::height << std::endl;
    }

    void Configuration::__parseAnims(std::atomic_uint &nbCounts)
    {
        __parseCreateAnims();
        nbCounts += 1u;
    }

    template <typename Map>
    void Configuration::__fillAnimationsInfos(const rapidjson::Value &animValue, Map &map)
    {
        for (const auto &v : animValue.GetArray()) {
            float speed = v.GetObject()["animinfo"].GetObject()["speed"].GetFloat();
            bool loop = v.GetObject()["animinfo"].GetObject()["loop"].GetBool();
            int repeat = v.GetObject()["animinfo"].GetObject()["repeat"].GetInt();
            auto status = static_cast<sfutils::AnimatedSprite::Status>(v.GetObject()["animinfo"].GetObject()["status"].GetInt());
            const std::string animation = v.GetObject()["animinfo"].GetObject()["animation"].GetString();
            AnimInfo info{loop, repeat, status, speed, animation};
            const auto &name = v.GetObject()["name"].GetString();
            _log(lg::Debug) << "\nAnimation : " << name << "\nAnimInfo : " << info << std::endl;
            map.emplace(std::make_pair(name, info));
        }
    }

    template <typename Map, typename AnimMap>
    void Configuration::__fillMultiAnim(const rapidjson::Value &value, Map &map, AnimMap &animMap)
    {
        using namespace sfutils;

        for (const auto &cur : value.GetObject()) {
            const auto &name = cur.name.GetString();
            std::vector<sfutils::AnimatedSprite> multiAnimTab;
            for (const auto &val : cur.value.GetArray()) {
                const rtype::AnimInfo &animInfo = map[val.GetString()];
                Animation animValue = std::string_view(animInfo.animation);
                TAnimation realValue = animValue;
                multiAnimTab.emplace_back(AnimatedSprite(&animations.get(realValue), animInfo.status,
                                                         sf::seconds(animInfo.speed), animInfo.loop, animInfo.repeat));
            }
            animMap.emplace(name, multiAnimTab);
        }
    }

    void Configuration::__parseCreateAnims()
    {
        using namespace std::string_literals;

        const auto path = cfg::animConfigPath + "create.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        const auto &createCFG = doc["createanim"];
        __fillAnimationsInfos(createCFG, cfg::create::animCreate);
        const auto &multi = doc["multianim"];
        __fillMultiAnim(multi, cfg::create::animCreate, cfg::create::multiAnimMap);
        cfg::ship::multiAnimMap = cfg::create::multiAnimMap;
    }

    void Configuration::__parseLangConfig()
    {
        __parseErrorConfig();
        __parseLoginConfig();
        __parseCreateConfig();
        __parseProfilConfig();
    }

    void Configuration::__parseLangConfig(std::atomic_uint &nbCounts)
    {
        __parseErrorConfig();
        nbCounts += 1;
        __parseLoginConfig();
        nbCounts += 1;
        __parseCreateConfig();
        nbCounts += 1;
        __parseProfilConfig();
        nbCounts += 1;
    }

    void Configuration::__parseErrorConfig()
    {
        using namespace std::string_literals;
        const auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "error.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&errorObject = doc["RTypeError"];
        unsigned int i = 0;
        for (auto error = errorObject.MemberBegin(); error != errorObject.MemberEnd(); ++error) {
            cfg::error::tab[i++] = error->value.GetString();
        }
    }

    void Configuration::__parseCreateConfig()
    {
        using namespace std::string_literals;
        using namespace cfg::create;

        const auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "create.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&createCFG = doc["createscene"];
        auto &&labelCFG = doc["createscene"]["labels"];
        chooseFactionImage = createCFG["choosefactionimage"].GetString();
        chooseFactionSize = {{0.f, createCFG["choosefactionimagesize"].GetArray()[0].GetFloat()},
                             {0.f, createCFG["choosefactionimagesize"].GetArray()[1].GetFloat()}};
        for (auto &&label : labelCFG.GetObject()) {
            std::pair<TCreateWidget, std::string> &labelPair = createLabel[label.name.GetString()];
            createLabel[label.name.GetString()] = std::make_pair(labelPair.first, label.value.GetString());
        }
    }

    void Configuration::__parseProfilConfig()
    {
        using namespace std::string_literals;
        using namespace cfg::profil;

        const auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "profile.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&value = doc["profilscene"]["labels"];
        for (auto &&label : value.GetObject()) {
            std::pair<TProfilWidget, std::string> &labelPair = profileLabel[label.name.GetString()];
            profileLabel[label.name.GetString()] = std::make_pair(labelPair.first, label.value.GetString());
        }
    }

    void Configuration::__parseLoginConfig()
    {
        using namespace std::string_literals;
        using namespace cfg::login;

        const auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "login.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&loginCFG = doc["loginscene"];
        auto &&labelCFG = doc["loginscene"]["labels"];

        for (auto &&label : labelCFG.GetObject()) {
            std::pair<TLoginWidget, std::string> &labelPair = logLabel[label.name.GetString()];
            logLabel[label.name.GetString()] = std::make_pair(labelPair.first, label.value.GetString());
        }

        logLabel["exittextmessage"].second.insert(0, 140, ' ');
        accountInformationImage = loginCFG["accountinformationimage"].GetString();
        accountNameLabelImage = loginCFG["accountnamelabelimage"].GetString();
        accountPasswordLabelImage = loginCFG["accountpasswordlabelimage"].GetString();
        accountInformationSize = {{0.f, loginCFG["accountinformationimagesize"].GetArray()[0].GetFloat()},
                                  {0.f, loginCFG["accountinformationimagesize"].GetArray()[1].GetFloat()}};
        accountNameLabelSize = {{0.f, loginCFG["accountnamelabelsize"].GetArray()[0].GetFloat()},
                                {0.f, loginCFG["accountnamelabelsize"].GetArray()[1].GetFloat()}};
        accountPasswordLabelSize = {{0.f, loginCFG["accountpasswordlabelsize"].GetArray()[0].GetFloat()},
                                    {0.f, loginCFG["accountpasswordlabelsize"].GetArray()[1].GetFloat()}};
    }

    void Configuration::__loadJSON(const std::string &path, rapidjson::Document &document)
    {
        using namespace std::string_literals;

        std::ifstream ifs(path);
        rapidjson::IStreamWrapper isw(ifs);
        document.ParseStream(isw);
        _log(lg::Info) << "Parsing file : " << path << std::endl;
        if (document.HasParseError()) {
            _log(lg::Error) << document.GetParseError() << " " << "Offset : " << document.GetErrorOffset() << std::endl;
            throw std::runtime_error("Parse file -> "s + path + " failed."s);
        }
    }

    void Configuration::__initAnims(std::atomic_uint &nbCounts)
    {
        __initFrames(Animation::BheetLv1, Sprite::BheetLv1, 11, 7, 75, nbCounts);
        __initFrames(Animation::KooyLv1, Sprite::KooyLv1, 11, 7, 75, nbCounts);
        __initFrames(Animation::MaulLv1, Sprite::MaulLv1, 11, 7, 75, nbCounts);
        __initFrames(Animation::BheetArrival, Sprite::BheetArrival, 12, 7, 80, nbCounts);
        __initFrames(Animation::KooyArrival, Sprite::KooyArrival, 12, 7, 80, nbCounts);
        __initFrames(Animation::MaulArrival, Sprite::MaulArrival, 12, 7, 80, nbCounts);
        __parseAnims(nbCounts);
    }

    void Configuration::__initFrames(TAnimation animation, TSprite sprite,
                                     unsigned int nbColumns,
                                     unsigned int nbLines,
                                     unsigned int nbAnims,
                                     std::atomic_uint &nbCounts) noexcept
    {
        auto &text = animations.load(animation, &textures.get(sprite));
        auto end = nbLines - 1;
        for (unsigned int i = 0; i < end; i++) {
            text.addFramesLine(nbColumns, nbLines, i, nbColumns);
        }
        auto animLastLine = (nbAnims - ((nbLines - 1) * nbColumns));
        text.addFramesLine(animLastLine, nbLines, nbLines - 1, nbColumns);
        _log(lg::Debug) << "Loading Animation " << Animation::toString(animation) << " successfully loaded."
                        << std::endl;
        nbCounts += 1;
    }
}