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
        _animSystem.clear();
        _textures.clear();
        _ettMgr.clear();
        __unbindPlayerCallbacks();
    }

    void DemoScene::draw() noexcept
    {
        _ettMgr.for_each<rtc::Sprite>([this](rtype::Entity &ett) {
            if (this->_debugMode)
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
            this->_win.draw(ett.getComponent<rtc::Sprite>().sprite);
        });

        _animSystem.draw();
    }

    void DemoScene::update(double timeSinceLastFrame) noexcept
    {
        ActionTarget::processEvents(timeSinceLastFrame);
        __bulletSystem(timeSinceLastFrame);
        _animSystem.update(timeSinceLastFrame);
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
            GameFactory::setEntityManager(&_ettMgr);
            __loadBulletSprite();
            __createGameObjects();
            __setPlayerCallbacks();
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
            demo::Sprite spriteValue = animName;
            __loadSprite(spriteValue);
            demo::AnimationSystem::Animation animValue = animName;
            sf::Vector2u spriteInfo{v.GetObject()["spriteInfo"].GetObject()["width"].GetUint(),
                                    v.GetObject()["spriteInfo"].GetObject()["height"].GetUint()};
            sf::Vector3<unsigned int> sheetInfo{v.GetObject()["sheetInfo"].GetObject()["rows"].GetUint(),
                                                v.GetObject()["sheetInfo"].GetObject()["column"].GetUint(),
                                                v.GetObject()["sheetInfo"].GetObject()["nbSprite"].GetUint()};
            _animSystem.loadAnimation(spriteValue, animValue, std::move(spriteInfo), std::move(sheetInfo));
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

    void DemoScene::__loadSprite(const demo::Sprite &val)
    {
        auto str = cfg::spritePath + val.toString() + ".png";
        _textures.load(val, str);
        _log(lg::Debug) << "Loading Image " << val.toString() + ".png" << " successfully loaded." << std::endl;
    }

    void DemoScene::__createGameObjects() noexcept
    {
        auto id = GameFactory::createPlayerSpaceShip(
            *_animSystem.get((demo::AnimationSystem::Animation::BheetLv1AttackTopDown)),
            _boundingBoxFactions["Bheet"],
            sf::Vector2f(200, 200));
        _playerID = id;
    }

    void DemoScene::__loadBulletSprite() noexcept
    {
        __loadSprite(demo::Sprite::Bullet);
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

    void DemoScene::__setPlayerCallbacks() noexcept
    {
        auto resetPosition = [](rtc::BoundingBox &box, const sf::Vector2f &texturePos) {
            box.AABB.left = texturePos.x + box.relativeAABB.left;
            box.AABB.top = texturePos.y + box.relativeAABB.top;
            box.shapeDebug.setPosition(sf::Vector2f{box.AABB.left, box.AABB.top});
        };

        setKeyCallback(cfg::player::Right, [this, &resetPosition](const sf::Event &, double timeSinceLastFrame) {
            auto[box, animation] = this->_ettMgr[_playerID].getComponents<rtc::BoundingBox, rtc::Animation>();
            sfutils::AnimatedSprite &anim = animation.anim;
            auto limitX = (cfg::game::width - box.AABB.width);
            if (box.AABB.left <= limitX) {
                anim.setPosition(static_cast<float>(anim.getPosition().x + (450 * timeSinceLastFrame)),
                                 anim.getPosition().y);
                resetPosition(box, anim.getPosition());
            }
        });

        setKeyCallback(cfg::player::Left, [this, &resetPosition](const sf::Event &, double timeSinceLastFrame) {
            auto[box, animation] = this->_ettMgr[_playerID].getComponents<rtc::BoundingBox, rtc::Animation>();
            sfutils::AnimatedSprite &anim = animation.anim;
            if (box.AABB.left >= 0)
                anim.setPosition(static_cast<float>(anim.getPosition().x - (450 * timeSinceLastFrame)),
                                 anim.getPosition().y);
            resetPosition(box, anim.getPosition());
        });

        setKeyCallback(cfg::player::Up, [this, &resetPosition](const sf::Event &, double timeSinceLastFrame) {
            auto[box, animation] = this->_ettMgr[_playerID].getComponents<rtc::BoundingBox, rtc::Animation>();
            sfutils::AnimatedSprite &anim = animation.anim;
            if (box.AABB.top >= 0) {
                anim.setPosition(anim.getPosition().x,
                                 static_cast<float>(anim.getPosition().y - (450 * timeSinceLastFrame)));
                resetPosition(box, anim.getPosition());
            }
        });

        setKeyCallback(cfg::player::Down, [this, &resetPosition](const sf::Event &, double timeSinceLastFrame) {
            auto[box, animation] = this->_ettMgr[_playerID].getComponents<rtc::BoundingBox, rtc::Animation>();
            sfutils::AnimatedSprite &anim = animation.anim;
            auto limitY = (cfg::game::height - box.AABB.height);
            if (box.AABB.top <= limitY) {
                anim.setPosition(anim.getPosition().x,
                                 static_cast<float>(anim.getPosition().y + (450 * timeSinceLastFrame)));
                resetPosition(box, anim.getPosition());
            }
        });

        setKeyCallback(cfg::player::SpaceShoot, [this](const sf::Event &, [[maybe_unused]] double timeSinceLastFrame) {
            const rtc::BoundingBox &boundingBox = this->_ettMgr[_playerID].getComponent<rtc::BoundingBox>();
            using namespace std::chrono_literals;
            auto res = std::chrono::steady_clock::now();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                std::chrono::duration_cast<std::chrono::milliseconds>(res - _lastShoot) > 200ms) {
                auto id = GameFactory::createBullet(_textures.get(demo::Sprite::Bullet), boundingBox.AABB,
                                                    Configuration::SoundEffect::Laser4);
                _evtMgr.emit<gutils::evt::PlaySoundEffect>(_ettMgr[id].getComponent<rtc::SoundEffect>().se,
                                                           _ettMgr[id].getComponent<rtc::SoundEffect>().buff);
                _lastShoot = std::chrono::steady_clock::now();
            }
        });
    }

    void DemoScene::__unbindPlayerCallbacks() noexcept
    {
        ActionTarget::clear();
    }
}