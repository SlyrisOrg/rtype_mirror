//
// Created by milerius on 22/12/17.
//

#include <chrono>
#include <rapidjson/istreamwrapper.h>
#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/scenes/DemoScene.hpp>
#include <rtype/config/ProfilConfig.hpp>
#include <rtype/gui/GUIManager.hpp>

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
        _ettMgr.clear();
    }

    void DemoScene::draw() noexcept
    {
        _ettMgr.for_each<rtc::Sprite>([this](rtype::Entity &ett) {
            if (this->_debugMode)
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
            this->_win.draw(ett.getComponent<rtc::Sprite>().sprite);
        });

        _ettMgr.for_each<rtc::Animation, rtc::BoundingBox>([this](rtype::Entity &ett) {
            if (this->_debugMode)
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
            _win.draw(ett.getComponent<rtc::Animation>().anim);
        });
    }

    void DemoScene::update(double timeSinceLastFrame) noexcept
    {
        sf::Time t1 = sf::seconds(static_cast<float>(timeSinceLastFrame));
        __inputSystem(timeSinceLastFrame);
        __bulletSystem(timeSinceLastFrame);
        __animationSystem(t1);
        _ettMgr.sweepEntities();
    }

    bool DemoScene::keyPressed(const gutils::evt::KeyPressed &evt) noexcept
    {
        if (evt.key == sf::Keyboard::F2) {
            _debugMode = !_debugMode;
        } else if (evt.key == sf::Keyboard::Escape) {
            _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Login);
        }
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
            __loadBulletSprite();
            GameFactory::setEntityManager(&_ettMgr);
            __createGameObjects();
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

    void DemoScene::__createGameObjects() noexcept
    {
        GameFactory::createPlayerSpaceShip(_animations.get(Animation::BheetLv1AttackTopDown),
                                           _boundingBoxFactions["Bheet"],
                                           sf::Vector2f(200, 200));
    }

    void DemoScene::__animationSystem(const sf::Time &time) noexcept
    {
        _ettMgr.for_each<rtc::Animation, rtc::BoundingBox>([&time](rtype::Entity &ett) {
            sfutils::AnimatedSprite &anim = ett.getComponent<rtc::Animation>().anim;
            anim.update(time);
        });
    }

    void DemoScene::__inputSystem(double timeSinceLastFrame) noexcept
    {
        auto resetPosition = [](rtc::BoundingBox &box, const sf::Vector2f &texturePos) {
            box.AABB.left = texturePos.x + box.relativeAABB.left;
            box.AABB.top = texturePos.y + box.relativeAABB.top;
            box.shapeDebug.setPosition(sf::Vector2f{box.AABB.left, box.AABB.top});
        };
        _ettMgr.for_each<rtc::Player>([this, &timeSinceLastFrame, &resetPosition](rtype::Entity &ett) {
            auto &anim = ett.getComponent<rtc::Animation>().anim;
            auto &boundingBox = ett.getComponent<rtc::BoundingBox>();
            auto limitX = (cfg::game::width - boundingBox.AABB.width);
            auto limitY = (cfg::game::height - boundingBox.AABB.height);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && boundingBox.AABB.left <= limitX) {
                anim.setPosition(static_cast<float>(anim.getPosition().x + (450 * timeSinceLastFrame)),
                                 anim.getPosition().y);
                resetPosition(boundingBox, anim.getPosition());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && boundingBox.AABB.left >= 0) {
                anim.setPosition(static_cast<float>(anim.getPosition().x - (450 * timeSinceLastFrame)),
                                 anim.getPosition().y);
                resetPosition(boundingBox, anim.getPosition());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && boundingBox.AABB.top <= limitY) {
                anim.setPosition(anim.getPosition().x,
                                 static_cast<float>(anim.getPosition().y + (450 * timeSinceLastFrame)));
                resetPosition(boundingBox, anim.getPosition());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && boundingBox.AABB.top >= 0) {
                anim.setPosition(anim.getPosition().x,
                                 static_cast<float>(anim.getPosition().y - (450 * timeSinceLastFrame)));
                resetPosition(boundingBox, anim.getPosition());
            }
            using namespace std::chrono_literals;
            auto res = std::chrono::steady_clock::now();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                std::chrono::duration_cast<std::chrono::milliseconds>(res - _lastShoot) > 200ms) {
                GameFactory::createBullet(_textures.get(Sprite::Bullet), boundingBox.AABB);
                _lastShoot = std::chrono::steady_clock::now();
            }
        });
    }

    void DemoScene::__loadBulletSprite() noexcept
    {
        __loadSprite(Sprite::Bullet);
    }

    void DemoScene::__bulletSystem(double timeSinceLastFrame) noexcept
    {
        _ettMgr.for_each<rtc::Bullet, rtc::Sprite, rtc::BoundingBox>([&timeSinceLastFrame](Entity &ett) {
            auto &cmp = ett.getComponent<rtc::Sprite>();
            auto &box = ett.getComponent<rtc::BoundingBox>();
            cmp.sprite.setPosition(static_cast<float>(box.AABB.left + (600 * timeSinceLastFrame)), box.AABB.top);
            box.AABB.left = cmp.sprite.getPosition().x;
            box.AABB.top = cmp.sprite.getPosition().y;
            box.shapeDebug.setPosition(cmp.sprite.getPosition());
            if (cmp.sprite.getPosition().x >= cfg::game::width) {
                ett.mark();
            }
        });
    }
}