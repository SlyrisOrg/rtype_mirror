//
// Created by roman sztergbaum on 16/09/2017.
//

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rtype/config/Configuration.hpp>
#include <fstream>

//! Static initializations.
namespace rtype
{
    sfutils::ResourceManager<sf::Texture, Configuration::TSprite> Configuration::textures;
    sfutils::ResourceManager<sfutils::Animation, Configuration::TAnimation> Configuration::animations;
    sfutils::ResourceManager<sf::Music, Configuration::TMusic> Configuration::musics;
    sfutils::ResourceManager<sfe::Movie, Configuration::TVideo> Configuration::videos;
    sfutils::ResourceManager<sf::SoundBuffer, Configuration::TSndEffect> Configuration::effects;
    lg::Logger Configuration::log = lg::Logger{"Configuration", lg::Info};
}

//! Public functions
namespace rtype
{
    void Configuration::initialize()
    {
        __initConfig();
        __initTextures();
        __initMusics();
        __initSoundEffects();
        __initVideos();
        __initAnims();
    }
}

//! Private functions
namespace rtype
{
    void Configuration::__initConfig() noexcept
    {
        try {
            auto path = utils::unpackToString(cfg::configPath, "config.json");
            rapidjson::Document doc;
            __prepareParse(path, doc);
            __getLangGameConfig(doc);
            __getWindowSize(doc);
            __parseLangConfig();
        }
        catch (const std::runtime_error &error) {
            log(lg::Error) << error.what() << std::endl;
        }
    }

    void Configuration::__initTextures()
    {
        LOAD_RESOURCE(Sprite, textures, cur, cfg::spritePath);
    }

    void Configuration::__initMusics()
    {
        LOAD_MUSIC(Music, musics, cur, "music", cfg::musicPath);
    }

    void Configuration::__initVideos()
    {
        LOAD_VIDEO(Video, videos, cur);
    }

    void Configuration::__initSoundEffects()
    {
        LOAD_MUSIC(SoundEffect, effects, cur, "sound effect", cfg::soundEffectPath);
    }

    void Configuration::__getLangGameConfig(const rapidjson::Document &document) noexcept
    {
        auto &&value = document["gameconfig"];
        cfg::game::lang = value["lang"].GetString();
        if (cfg::game::lang != "en" && cfg::game::lang != "fr")
            cfg::game::lang = cfg::defaultLang;
        log(lg::Info) << "The language in game is : " << utils::LightRed << cfg::game::lang << utils::Reset
                      << std::endl;
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
        log(lg::Info) << "Window size : " << utils::LightRed << cfg::game::width << utils::Reset << ", "
                      << utils::LightRed << cfg::game::height << utils::Reset << std::endl;
    }

    void Configuration::__parseLangConfig()
    {
        __parseErrorConfig();
        __parseLoginConfig();
        __parseCreateConfig();
    }

    void Configuration::__parseErrorConfig()
    {
        auto path = utils::unpackToString(cfg::configPath, "lang/", cfg::game::lang, "/", "error.json");
        rapidjson::Document doc;
        __prepareParse(path, doc);
        auto &&value = doc["RTypeError"];
        int i = 0;
        for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin(); itr != value.MemberEnd(); itr++) {
            cfg::error::tab[i++] = itr->value.GetString();
        }
    }

    void Configuration::__parseCreateConfig()
    {
        auto path = utils::unpackToString(cfg::configPath, "lang/", cfg::game::lang, "/", "create.json");
        rapidjson::Document doc;
        __prepareParse(path, doc);
        auto &&value = doc["createscene"];
        cfg::create::chooseFactionImage = value["choosefactionimage"].GetString();
        cfg::create::chooseFactionSize = {{0.f, value["choosefactionimagesize"].GetArray()[0].GetFloat()},
                                          {0.f, value["choosefactionimagesize"].GetArray()[1].GetFloat()}};
    }

    void Configuration::__parseLoginConfig()
    {
        auto path = utils::unpackToString(cfg::configPath, "lang/", cfg::game::lang, "/", "login.json");
        rapidjson::Document doc;
        __prepareParse(path, doc);
        auto &&value = doc["loginscene"];
        cfg::login::loginButtonText = value["logbutton"].GetString();
        cfg::login::exitButtonText = value["exitbutton"].GetString();
        cfg::login::exitYesButtonText = value["exityesbutton"].GetString();
        cfg::login::exitNoButtonText = value["exitnobutton"].GetString();
        cfg::login::exitMessageText = value["exittextmessage"].GetString();
        cfg::login::exitMessageText.insert(0, 140, ' ');
        cfg::login::errorYesText = value["erroryesbutton"].GetString();
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

    void Configuration::__prepareParse(const std::string &path, rapidjson::Document &document)
    {
        std::ifstream ifs(path);
        rapidjson::IStreamWrapper isw(ifs);
        document.ParseStream(isw);
        log(lg::Info) << "Parsing file : " << utils::LightRed << path << utils::Reset << std::endl;
        if (document.HasParseError()) {
            log(lg::Error) << document.GetParseError() << " " << "Offset : " << document.GetErrorOffset()
                           << std::endl;
            throw std::runtime_error(utils::unpackToString("Parse file -> ", path, " failed."));
        }
    }

    void Configuration::__initAnims()
    {
        __initFrames(Animation::BheetLv1, Sprite::BheetLv1, 12, 5);
        __initFrames(Animation::KooyLv1, Sprite::KooyLv1, 12, 5);
        __initFrames(Animation::MaulLv1, Sprite::MaulLv1, 12, 5);
    }

    void Configuration::__initFrames(TAnimation animation, TSprite sprite, int x, int y) noexcept
    {
        auto &text = animations.load(animation, &textures.get(sprite));
        for (int i = 0; i < y; i++) {
            text.addFramesLine(x, y, i);
        }
        log(lg::Info) << "Loading Animation " << utils::Brown << Animation::toString(animation) << utils::Reset
                      << " successfully loaded." << std::endl;
    }
}