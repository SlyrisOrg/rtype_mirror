//
// Created by roman sztergbaum on 16/09/2017.
//

#include <atomic>
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

//Gay Compiler
#ifdef USING_MSVC
#undef GetObject
#endif

//! Static initializations.
namespace rtype
{
    using namespace std::string_literals;
    sfutils::ResourceManager<sf::Texture, Configuration::TSprite> Configuration::textures;
    sfutils::ResourceManager<sfutils::Animation, Configuration::TAnimation> Configuration::animations;
    sfutils::ResourceManager<sf::Music, Configuration::TMusic> Configuration::musics;
    sfutils::ResourceManager<sfe::Movie, Configuration::TVideo> Configuration::videos;
    sfutils::ResourceManager<sf::SoundBuffer, Configuration::TSndEffect> Configuration::effects;
    lg::Logger Configuration::log = lg::Logger{"Configuration"s, lg::Debug};
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
            auto path = cfg::configPath + "config.json"s;
            rapidjson::Document doc;
            __loadJSON(path, doc);
            auto &&value = doc["gameconfig"];
            log(lg::Info) << value["lang"].GetString() << std::endl;
            value["lang"].SetString(lang.c_str(), static_cast<rapidjson::SizeType>(lang.length()));
            log(lg::Info) << value["lang"].GetString() << std::endl;
            std::ofstream ofs(path);
            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            doc.Accept(writer);
            __getLangGameConfig(doc);
            __parseLangConfig();
        }
        catch (const std::runtime_error &err) {
            log(lg::Error) << err.what() << std::endl;
        }
    }
}

//! Private functions
namespace rtype
{
    void Configuration::__initConfig(std::atomic_uint &currentNbFiles) noexcept
    {
        try {
            auto path = cfg::configPath + "config.json"s;
            rapidjson::Document doc;
            __loadJSON(path, doc);
            __getLangGameConfig(doc);
            __getWindowSize(doc);
            currentNbFiles += 1u;
            __parseLangConfig(currentNbFiles);
        }
        catch (const std::runtime_error &error) {
            log(lg::Error) << error.what() << std::endl;
        }
    }

    void Configuration::__initTextures(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<Sprite>(cfg::spritePath, ".png"s, textures, "textures"sv, nbCounts);
    }

    void Configuration::__initMusics(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<Music>(cfg::musicPath, ".ogg"s, musics, "musics"sv, nbCounts);
    }

    void Configuration::__initVideos(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<Video>(cfg::videoPath, ".avi"s, videos, "video"sv, nbCounts);
    }

    void Configuration::__initSoundEffects(std::atomic_uint &nbCounts)
    {
        using namespace std::string_view_literals;
        __initResources<SoundEffect>(cfg::soundEffectPath, ".ogg"s, effects, "sounds effect"sv, nbCounts);
    }

    void Configuration::__getLangGameConfig(const rapidjson::Document &document) noexcept
    {
        auto &&value = document["gameconfig"];
        cfg::game::lang = value["lang"].GetString();
        if (cfg::game::lang != "en"s && cfg::game::lang != "fr"s)
            cfg::game::lang = cfg::defaultLang;
        log(lg::Info) << "The language in game is : " << cfg::game::lang << std::endl;
    }

    void Configuration::__getWindowSize(const rapidjson::Document &document) noexcept
    {
        auto &&value = document["gameconfig"];
        cfg::game::width = value["width"].GetUint();
        cfg::game::height = value["height"].GetUint();
        if (cfg::game::width > cfg::maxWidth)
            cfg::game::width = cfg::maxWidth;
        if (cfg::game::height > cfg::maxHeight)
            cfg::game::width = cfg::maxHeight;
        log(lg::Info) << "Window size : " << cfg::game::width << ", " << cfg::game::height << std::endl;
    }

    void Configuration::__parseAnims(std::atomic_uint &nbCounts)
    {
        __parseCreateAnims();
        nbCounts += 1u;
    }

    template <typename Map>
    void Configuration::__fillAnimationsInfos(const rapidjson::Value &value, Map &map)
    {
        for (const auto &v : value.GetArray()) {
            const auto &name = v.GetObject()["name"];
            float speed = v.GetObject()["animinfo"].GetObject()["speed"].GetFloat();
            bool loop = v.GetObject()["animinfo"].GetObject()["loop"].GetBool();
            int repeat = v.GetObject()["animinfo"].GetObject()["repeat"].GetInt();
            auto status = static_cast<sfutils::AnimatedSprite::Status>(v.GetObject()["animinfo"].GetObject()["status"].GetInt());
            std::string animation = v.GetObject()["animinfo"].GetObject()["animation"].GetString();
            AnimInfo info{loop, repeat, status, speed, animation};
            log(lg::Debug) << "\nAnimation : " << name.GetString() << "\nAnimInfo : " << info << std::endl;
            map.emplace(std::make_pair(name.GetString(), info));
        }
    }

    template <typename Map, typename AnimMap>
    void Configuration::__fillMultiAnim(const rapidjson::Value &value, Map &map, AnimMap &animMap)
    {
        for (const auto &cur : value.GetObject()) {
            const auto &name = cur.name.GetString();
            std::vector<sfutils::AnimatedSprite> multiAnimTab;
            for (const auto &val : cur.value.GetArray()) {
                const rtype::AnimInfo &animInfo = map[val.GetString()];
                Configuration::Animation animValue = std::string_view(animInfo.animation);
                Configuration::TAnimation realValue = animValue;
                sfutils::AnimatedSprite spr(&Configuration::animations.get(realValue), animInfo.status,
                                            sf::seconds(animInfo.speed), animInfo.loop, animInfo.repeat);
                multiAnimTab.emplace_back(spr);
            }
            animMap.emplace(name, multiAnimTab);
        }
    }

    void Configuration::__parseCreateAnims()
    {
        using namespace std::string_literals;
        auto path = cfg::animConfigPath + "create.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        const rapidjson::Value &value = doc["createanim"];
        __fillAnimationsInfos(value, cfg::create::animCreate);
        const rapidjson::Value &multi = doc["multianim"];
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
        auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "error.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&value = doc["RTypeError"];
        int i = 0;
        for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin(); itr != value.MemberEnd(); itr++) {
            cfg::error::tab[i++] = itr->value.GetString();
        }
    }

    void Configuration::__parseCreateConfig()
    {
        using namespace std::string_literals;
        auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "create.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&value = doc["createscene"];
        auto &&labval = doc["createscene"]["labels"];
        cfg::create::chooseFactionImage = value["choosefactionimage"].GetString();
        cfg::create::chooseFactionSize = {{0.f, value["choosefactionimagesize"].GetArray()[0].GetFloat()},
                                          {0.f, value["choosefactionimagesize"].GetArray()[1].GetFloat()}};
        for (auto &&label : labval.GetObject()) {
            const auto &v = cfg::create::createLabel[label.name.GetString()];
            cfg::create::createLabel[label.name.GetString()] = std::make_pair(v.first, label.value.GetString());
        }
    }

    void Configuration::__parseProfilConfig()
    {
        using namespace std::string_literals;
        auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "profile.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&value = doc["profilscene"]["labels"];
        for (auto &&label : value.GetObject()) {
            const auto &v = cfg::profil::profileLabel[label.name.GetString()];
            cfg::profil::profileLabel[label.name.GetString()] = std::make_pair(v.first, label.value.GetString());
        }
    }

    void Configuration::__parseLoginConfig()
    {
        auto path = cfg::configPath + "lang/"s + cfg::game::lang + "/"s + "login.json"s;
        rapidjson::Document doc;
        __loadJSON(path, doc);
        auto &&value = doc["loginscene"];
        auto &&labval = doc["loginscene"]["labels"];
        for (auto &&label : labval.GetObject()) {
            const auto &v = cfg::login::logLabel[label.name.GetString()];
            cfg::login::logLabel[label.name.GetString()] = std::make_pair(v.first, label.value.GetString());
        }
        cfg::login::logLabel["exittextmessage"].second.insert(0, 140, ' ');
        cfg::login::accountInformationImage = value["accountinformationimage"].GetString();
        cfg::login::accountNameLabelImage = value["accountnamelabelimage"].GetString();
        cfg::login::accountPasswordLabelImage = value["accountpasswordlabelimage"].GetString();
        cfg::login::accountInformationSize = {{0.f, value["accountinformationimagesize"].GetArray()[0].GetFloat()},
                                              {0.f, value["accountinformationimagesize"].GetArray()[1].GetFloat()}};
        cfg::login::accountNameLabelSize = {{0.f, value["accountnamelabelsize"].GetArray()[0].GetFloat()},
                                            {0.f, value["accountnamelabelsize"].GetArray()[1].GetFloat()}};
        cfg::login::accountPasswordLabelSize = {{0.f, value["accountpasswordlabelsize"].GetArray()[0].GetFloat()},
                                                {0.f, value["accountpasswordlabelsize"].GetArray()[1].GetFloat()}};
    }

    void Configuration::__loadJSON(const std::string &path, rapidjson::Document &document)
    {
        std::ifstream ifs(path);
        rapidjson::IStreamWrapper isw(ifs);
        document.ParseStream(isw);
        log(lg::Info) << "Parsing file : " << path << std::endl;
        if (document.HasParseError()) {
            log(lg::Error) << document.GetParseError() << " " << "Offset : " << document.GetErrorOffset() << std::endl;
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
        log(lg::Debug) << "Loading Animation " << Animation::toString(animation) << " successfully loaded."
                       << std::endl;
        nbCounts += 1;
    }
}