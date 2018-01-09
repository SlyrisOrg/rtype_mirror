//
// Created by milerius on 22/12/17.
//

#include <rtype/scenes/DemoScene.hpp>
#include <rtype/config/ProfilConfig.hpp>
#include <rtype/gui/GUIManager.hpp>
#include <rapidjson/istreamwrapper.h>

namespace rtype
{
    void DemoScene::enter() noexcept
    {
        showEnterScene();
        __configure();
    }

    void DemoScene::resume() noexcept
    {
        showResumeScene();
    }

    void DemoScene::pause() noexcept
    {
        showPauseScene();
    }

    void DemoScene::leave() noexcept
    {
        showLeavingScene();
        _boundingBoxFactions.clear();
        _animations.clear();
        _textures.clear();
    }

    void DemoScene::draw() noexcept
    {
    }

    void DemoScene::update([[maybe_unused]] double timeSinceLastFrame) noexcept
    {
    }

    bool DemoScene::keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept
    {
        return false;
    }

    bool DemoScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool DemoScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool DemoScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool DemoScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void DemoScene::__configure() noexcept
    {
        auto start = __setGUI();
        if (start) {
            __parseConfig("Bheet");
        }
    }

    bool DemoScene::__setGUI() noexcept
    {
        using namespace cfg::play;
        return GUIManager::setGUI<UIWidgets, nbWidgets>(UILayout, _gui, _log);
    }

    void DemoScene::__parseConfig(const std::string &faction)
    {
        using namespace std::string_literals;
        try {
            const auto path = cfg::animConfigPath + faction + "Lv"s + cfg::profil::level + "AttackInGame.json";
            rapidjson::Document doc;
            std::ifstream ifs(path);
            rapidjson::IStreamWrapper isw(ifs);
            doc.ParseStream(isw);
            _log(lg::Info) << "Parsing file: " << path << std::endl;
            if (doc.HasParseError()) {
                _log(lg::Error) << doc.GetParseError() << " " << "Offset : " << doc.GetErrorOffset() << std::endl;
                throw std::runtime_error("Parse file -> "s + path + " failed."s);
            }
            __parseConfigInside(doc, faction);
        }
        catch (const std::exception &error) {
            _log(lg::Error) << error.what() << std::endl;
            _evtMgr.emit<gutils::evt::GameShutdown>();
        }
    }

    void DemoScene::__parseConfigInside(const rapidjson::Document &doc, const std::string &faction)
    {
        const auto &animCFG = doc["Animation"];
        const auto &boxCFG = doc["Box"];
        for (const auto &v : animCFG.GetArray()) {
            std::string_view animName = v.GetObject()["name"].GetString();
            DemoScene::Sprite spriteValue = animName;
            __loadSprite(spriteValue);
            DemoScene::Animation animValue = animName;
            sf::Vector2u spriteInfo{v.GetObject()["spriteInfo"].GetObject()["width"].GetUint(),
                                    v.GetObject()["spriteInfo"].GetObject()["height"].GetUint()};
            sf::Vector3<unsigned int> sheetInfo{v.GetObject()["sheetInfo"].GetObject()["rows"].GetUint(),
                                                v.GetObject()["sheetInfo"].GetObject()["column"].GetUint(),
                                                v.GetObject()["sheetInfo"].GetObject()["nbSprite"].GetUint()};
            __loadAnimation(spriteValue, animValue, std::move(spriteInfo), std::move(sheetInfo));
        }
        const auto &res = _boundingBoxFactions.emplace(faction,
                                                       sf::IntRect{
                                                           boxCFG.GetObject()["position"].GetObject()["x"].GetInt(),
                                                           boxCFG.GetObject()["position"].GetObject()["y"].GetInt(),
                                                           boxCFG.GetObject()["size"].GetObject()["width"].GetInt(),
                                                           boxCFG.GetObject()["size"].GetObject()["height"].GetInt()});
        _log(lg::Debug) << "\nFaction: " << faction
                        << "\nBox: { x: "
                        << res.first->second.left << " y: "
                        << res.first->second.top << " width: "
                        << res.first->second.width << " height: "
                        << res.first->second.height
                        << " }" << std::endl;
    }

    void DemoScene::__loadAnimation(const Sprite &sprite,
                                    const Animation &anim,
                                    [[maybe_unused]] sf::Vector2<unsigned int> &&sprInfo,
                                    sf::Vector3<unsigned int> &&sheetInfo)
    {
        __initFrames(anim, sprite, sheetInfo.y, sheetInfo.x, sheetInfo.z);
    }

    void DemoScene::__initFrames(const Animation &anim, const Sprite &sprite,
                                 unsigned int nbColumns,
                                 unsigned int nbLines,
                                 unsigned int nbAnims)
    {
        auto &text = _animations.load(anim, &_textures.get(sprite));
        auto end = nbLines - 1;
        for (unsigned int i = 0; i < end; i++) {
            text.addFramesLine(nbColumns, nbLines, i, nbColumns);
        }
        auto animLastLine = (nbAnims - ((nbLines - 1) * nbColumns));
        text.addFramesLine(animLastLine, nbLines, nbLines - 1, nbColumns);
        _log(lg::Debug) << "Loading Animation " << anim.toString() << " successfully loaded." << std::endl;
    }

    void DemoScene::__loadSprite(const DemoScene::Sprite &val)
    {
        auto str = cfg::spritePath + val.toString() + ".png";
        _textures.load(val, str);
        _log(lg::Debug) << "Loading Image " << val.toString() + ".png" << " successfully loaded." << std::endl;
    }
}