//
// Created by milerius on 22/12/17.
//

#include <chrono>
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
        _quadTree.clear();
        _fieldSystem.clear();
        _ettMgr.clear();
        __unbindPlayerCallbacks();
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::BattleExtInstrumentalAmb);
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::AbandonedFacilitiesInstrumentalAmb);
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::SpaceshipBridgeInstrumentalAmb);
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::MarketInstrumentalAmb);
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::DarkVoidAmb);
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::CitadelInstrumentalAmb);
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::CloseStarInstrumentalAmb);
    }

    void DemoScene::draw() noexcept
    {
        if (this->_debugMode) {
            _win.draw(_quadTree.getRoot()._shapeDebug);
        }

        auto spriteDrawer = [this](rtype::Entity &ett) {
            if (this->_debugMode && ett.hasComponent<rtc::BoundingBox>()) {
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);

                auto node = _quadTree.getNode(ett.getID());
                if (node) {
                    _win.draw(node->_shapeDebug);
                }
                else {
                    _log(lg::Error) << "not a member of the quadtree" << std::endl;
                }
            }
            this->_win.draw(ett.getComponent<rtc::Sprite>().sprite);
        };

        auto animDrawer = [this](rtype::Entity &ett) {
            if (this->_debugMode) {
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
                auto node = _quadTree.getNode(ett.getID());
                if (node) {
                    _win.draw(node->_shapeDebug);
                }
                else {
                    _log(lg::Error) << "not a member of the quadtree" << std::endl;
                }
            }
            _win.draw(ett.getComponent<rtc::Animation>().anim);
        };

        _ettMgr.for_each<rtc::Sprite, rtc::StarFieldLayer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::StarFieldLayer>(animDrawer);

        _ettMgr.for_each<rtc::Sprite, rtc::Fog3Layer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::Fog3Layer>(animDrawer);

        _ettMgr.for_each<rtc::Sprite, rtc::Planet2Layer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::Planet2Layer>(animDrawer);

        _ettMgr.for_each<rtc::Sprite, rtc::Fog2Layer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::Fog2Layer>(animDrawer);

        _ettMgr.for_each<rtc::Sprite, rtc::Planet1Layer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::Planet1Layer>(animDrawer);

        _ettMgr.for_each<rtc::Sprite, rtc::GameFieldLayer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::GameFieldLayer>(animDrawer);

        _ettMgr.for_each<rtc::Sprite, rtc::Fog1Layer>(spriteDrawer);
        _ettMgr.for_each<rtc::Animation, rtc::Fog1Layer>(animDrawer);
    }

    void DemoScene::update(double timeSinceLastFrame) noexcept
    {
        ActionTarget::processEvents(timeSinceLastFrame);
        __bulletSystem(timeSinceLastFrame);
        _collisionSystem.update(timeSinceLastFrame);
        _fieldSystem.update(timeSinceLastFrame);
        _scenario.update(timeSinceLastFrame);
        if (_ettMgr.nbEntities() > 0) {
            rtc::Stat &stat = _ettMgr[_playerID].getComponent<rtc::Stat>();
            static_cast<CEGUI::ProgressBar &>(_gui[ui::UIWidgets::PlayerPV]).setProgress(
                    static_cast<float>(stat.hp) / stat.hpMax);
            _ettMgr.for_each<rtc::Enemy, rtc::Stat>([this](Entity &ett){
                auto &statt = ett.getComponent<rtc::Stat>();
                if (statt.hpBar)
                    statt.hpBar->setProgress(static_cast<float>(statt.hp) / statt.hpMax);
            });
            if (stat.hp <= 0)
                _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Login);
        }
        if (_ettMgr.nbEntities() > 0) {
            _animSystem.update(timeSinceLastFrame);
            _ettMgr.for_each<rtc::BoundingBox>([this](Entity &ett){
                if (ett.isMarked())
                    _quadTree.remove(ett.getID());
            });
        }
        _ettMgr.sweepEntities();
    }

    bool DemoScene::keyPressed(const gutils::evt::KeyPressed &evt) noexcept
    {
        if (evt.key == sf::Keyboard::F2) {
            _debugMode = !_debugMode;
        } else if (evt.key == sf::Keyboard::Escape) {
            _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Login);
        }

        if (evt.key == sf::Keyboard::Up
            || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
                (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))) {
            _ettMgr[_playerID].getComponent<rtc::Movement>().dir = rtc::Direction::North;
            _animSystem.changePlayerAnim();
        } else if (evt.key == sf::Keyboard::Down
                   || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
                       (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))) {
            _ettMgr[_playerID].getComponent<rtc::Movement>().dir = rtc::Direction::South;
            _animSystem.changePlayerAnim();
        }
        return false;
    }

    bool DemoScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        if (evt.key == sf::Keyboard::Up) {
            _ettMgr[_playerID].getComponent<rtc::Movement>().dir = rtc::Direction::None;
            _animSystem.changePlayerAnim();
        } else if (evt.key == sf::Keyboard::Down) {
            _ettMgr[_playerID].getComponent<rtc::Movement>().dir = rtc::Direction::None;
            _animSystem.changePlayerAnim();
        }
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
            GameFactory::setQuadTree(&_quadTree);
            GameFactory::setLuaManager(&_luaMgr);
            _registerAdditionalLuaFunctions();
            __loadBulletSprite();
            __loadStarSprite();
            __loadFogSprite();
            __loadPlanetSprite();
            __loadEnemySprite();
            _fieldSystem.configure();
            __loadScript();
            _scenario.configure("assets/config/level/levelDemo.json");
            __createGameObjects();
            __setPlayerCallbacks();
            //_evtMgr.emit<gutils::evt::PlayMusic>(Configuration::Music::CitadelInstrumentalAmb, true);
        }
    }

    bool DemoScene::__setGUI() noexcept
    {
        using namespace cfg::play;
        return GUIManager::setGUI<ui::UIWidgets, nbWidgets>(UILayout, _gui, _log);
    }

    void __subscribeEvents() noexcept
    {/*
        using namespace rtype::ui;
        using pb = CEGUI::PushButton;
        using evt = CEGUI::Event;
        using Ds = DemoScene;
      */
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
        const auto &statCFG = doc["Stat"].GetObject();
        const auto &resStat = _statFactions.emplace(faction, rtc::Stat{statCFG["hp"].GetUint(),
                                                                       statCFG["attack"].GetUint(),
                                                                       statCFG["defense"].GetUint(),
                                                                       statCFG["speed"].GetUint(),
                                                                       statCFG["shield"].GetUint(),
                                                                       &static_cast<CEGUI::ProgressBar &>(
                                                                               _gui[ui::UIWidgets::PlayerPV])});
        _log(lg::Debug) << "\nFaction: " << faction
                        << "\nBox: { x: "
                        << res.first->second.left << " y: "
                        << res.first->second.top << " width: "
                        << res.first->second.width << " height: "
                        << res.first->second.height << " }"
                        << "\nStat: { hp : "
                        << resStat.first->second.hp << " atk :"
                        << resStat.first->second.attack << " def : "
                        << resStat.first->second.defense << " spd : "
                        << resStat.first->second.speed << " shield : "
                        << resStat.first->second.shield << " }"
                        << std::endl;
    }

    void DemoScene::__loadSprite(const demo::Sprite &val)
    {
        auto str = cfg::spritePath + val.toString() + ".png";
        _textures.load(val, str);
        _log(lg::Debug) << "Loading Image " << val.toString() + ".png" << " successfully loaded." << std::endl;
    }

    void DemoScene::__createGameObjects() noexcept
    {
        using as = demo::AnimationSystem;
        std::unordered_map<int, std::pair<int, int>> map
            {
                {
                    as::Animation::BheetLv1AttackTopDown,
                    {as::Animation::BheetLv1AttackTurnUp,      as::Animation::BheetLv1AttackTurnDown}
                },
                {
                    as::Animation::BheetLv1AttackTurnUp,
                    {as::Animation::BheetLv1AttackTurnUp,      as::Animation::BheetLv1AttackRedressUp}
                },
                {
                    as::Animation::BheetLv1AttackTurnDown,
                    {as::Animation::BheetLv1AttackRedressDown, as::Animation::BheetLv1AttackTurnDown}
                },
                {
                    as::Animation::BheetLv1AttackRedressUp,
                    {as::Animation::BheetLv1AttackTurnUp,      as::Animation::BheetLv1AttackRedressUp}
                },
                {
                    as::Animation::BheetLv1AttackRedressDown,
                    {as::Animation::BheetLv1AttackRedressDown, as::Animation::BheetLv1AttackTurnDown}
                }
            };
        auto id = GameFactory::createPlayerSpaceShip(map,
                                                     _animSystem.get(
                                                         (demo::AnimationSystem::Animation::BheetLv1AttackTopDown)),
                                                     _boundingBoxFactions["Bheet"],
                                                     sf::Vector2f(200, 200),
                                                     _statFactions.at("Bheet"));
        _quadTree.insert(id);
        _playerID = id;
        _animSystem.setPlayerID(_playerID);
        _ettMgr[_playerID].getComponent<rtc::Animation>().currentAnim = as::Animation::BheetLv1AttackTopDown;
    }

    void DemoScene::__loadPlanetSprite() noexcept
    {
        __loadSprite(demo::Sprite::BluePlanet1);
        __loadSprite(demo::Sprite::BluePlanet2);
        __loadSprite(demo::Sprite::BluePlanet3);
        __loadSprite(demo::Sprite::BluePlanet4);
        __loadSprite(demo::Sprite::BluePlanet5);
        __loadSprite(demo::Sprite::BluePlanet6);
        __loadSprite(demo::Sprite::BluePlanet7);
        __loadSprite(demo::Sprite::BluePlanet8);
        __loadSprite(demo::Sprite::BluePlanet9);
        __loadSprite(demo::Sprite::BluePlanet10);
        __loadSprite(demo::Sprite::BluePlanet11);
        __loadSprite(demo::Sprite::BluePlanet12);
        __loadSprite(demo::Sprite::BluePlanet13);
        __loadSprite(demo::Sprite::BluePlanet14);
        __loadSprite(demo::Sprite::BluePlanet15);
        __loadSprite(demo::Sprite::BluePlanet16);
        __loadSprite(demo::Sprite::BluePlanet17);
        __loadSprite(demo::Sprite::BluePlanet18);
        __loadSprite(demo::Sprite::BluePlanet19);
        __loadSprite(demo::Sprite::RedPlanet1);
        __loadSprite(demo::Sprite::RedPlanet2);
        __loadSprite(demo::Sprite::RedPlanet3);
        __loadSprite(demo::Sprite::RedPlanet4);
        __loadSprite(demo::Sprite::RedPlanet5);
        __loadSprite(demo::Sprite::RedPlanet6);
        __loadSprite(demo::Sprite::RedPlanet7);
        __loadSprite(demo::Sprite::RedPlanet8);
        __loadSprite(demo::Sprite::RedPlanet9);
        __loadSprite(demo::Sprite::RedPlanet10);
        __loadSprite(demo::Sprite::RedPlanet11);
        __loadSprite(demo::Sprite::RedPlanet12);
        __loadSprite(demo::Sprite::RedPlanet13);
        __loadSprite(demo::Sprite::RedPlanet14);
        __loadSprite(demo::Sprite::RedPlanet15);
        __loadSprite(demo::Sprite::RedPlanet16);
        __loadSprite(demo::Sprite::RedPlanet17);
        __loadSprite(demo::Sprite::RedPlanet18);
        __loadSprite(demo::Sprite::RedPlanet19);
        __loadSprite(demo::Sprite::GreenPlanet1);
        __loadSprite(demo::Sprite::GreenPlanet2);
        __loadSprite(demo::Sprite::GreenPlanet3);
        __loadSprite(demo::Sprite::GreenPlanet4);
        __loadSprite(demo::Sprite::GreenPlanet5);
        __loadSprite(demo::Sprite::GreenPlanet6);
        __loadSprite(demo::Sprite::GreenPlanet7);
        __loadSprite(demo::Sprite::GreenPlanet8);
        __loadSprite(demo::Sprite::GreenPlanet9);
        __loadSprite(demo::Sprite::GreenPlanet10);
        __loadSprite(demo::Sprite::GreenPlanet11);
        __loadSprite(demo::Sprite::GreenPlanet12);
        __loadSprite(demo::Sprite::GreenPlanet13);
        __loadSprite(demo::Sprite::GreenPlanet14);
    }

    void DemoScene::__loadStarSprite() noexcept
    {
        __loadSprite(demo::Sprite::WhiteStar1);
        __loadSprite(demo::Sprite::WhiteStar2);
        __loadSprite(demo::Sprite::GreenStar1);
        __loadSprite(demo::Sprite::GreenStar2);
        __loadSprite(demo::Sprite::GreenStar3);
        __loadSprite(demo::Sprite::GreenStar4);
        __loadSprite(demo::Sprite::GreenStar5);
        __loadSprite(demo::Sprite::BlueStar1);
        __loadSprite(demo::Sprite::BlueStar2);
        __loadSprite(demo::Sprite::BlueStar3);
        __loadSprite(demo::Sprite::BlueStar4);
        __loadSprite(demo::Sprite::BlueStar5);
        __loadSprite(demo::Sprite::BlueStar6);
        __loadSprite(demo::Sprite::BlueStar7);
        __loadSprite(demo::Sprite::RedStar1);
        __loadSprite(demo::Sprite::RedStar2);
        __loadSprite(demo::Sprite::RedStar3);
        __loadSprite(demo::Sprite::RedStar4);
        __loadSprite(demo::Sprite::RedStar5);
        __loadSprite(demo::Sprite::RedStar6);
        __loadSprite(demo::Sprite::RedStar7);
    }

    void DemoScene::__loadFogSprite() noexcept
    {
        __loadSprite(demo::Sprite::GreenFog1);
        __loadSprite(demo::Sprite::GreenFog2);
        __loadSprite(demo::Sprite::RedFog1);
        __loadSprite(demo::Sprite::RedFog2);
        __loadSprite(demo::Sprite::BlueFog1);
        __loadSprite(demo::Sprite::BlueFog2);
        __loadSprite(demo::Sprite::BlueFog3);
    }

    void DemoScene::__loadBulletSprite() noexcept
    {
        __loadSprite(demo::Sprite::Bullet);
    }

    void DemoScene::__loadEnemySprite() noexcept
    {
        __loadSprite(demo::Sprite::Boss);
        __loadSprite(demo::Sprite::Minion1);
        __loadSprite(demo::Sprite::Minion2);
        __loadSprite(demo::Sprite::Minion3);
        __loadSprite(demo::Sprite::Minion4);
        __loadSprite(demo::Sprite::Minion5);
    }

    void DemoScene::__loadScript() noexcept
    {
        _luaMgr.loadScript("player.lua");
        _luaMgr.loadScript("stayAndShoot.lua");
        _luaMgr.loadScript("standardAttackBullet.lua");
    }

    void DemoScene::__bulletSystem(double timeSinceLastFrame) noexcept
    {
        _ettMgr.for_each<rtc::Bullet, rtc::Sprite, rtc::BoundingBox>([this, &timeSinceLastFrame](Entity &ett) {
            auto &cmp = ett.getComponent<rtc::Sprite>();
            auto &box = ett.getComponent<rtc::BoundingBox>();
            box.setPosition(static_cast<float>(box.AABB.left + (1100 * timeSinceLastFrame)), box.AABB.top);
            cmp.sprite.setPosition(box.getPosition());
            _quadTree.move(ett.getID());
            if (box.getPosition().x >= cfg::game::width) {
                _quadTree.remove(ett.getID());
                ett.mark();
            }
        });
    }

    void DemoScene::__setPlayerCallbacks() noexcept
    {
        setKeyCallback(cfg::player::Right, [this](const sf::Event &, double timeSinceLastFrame) {
            _luaMgr[_ettMgr[_playerID].getComponent<rtc::Lua>().tableName]["moveRight"](_playerID, timeSinceLastFrame);
            //_luaMgr.executeFunction("moveRight", _playerID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::Left, [this](const sf::Event &, double timeSinceLastFrame) {
            _luaMgr[_ettMgr[_playerID].getComponent<rtc::Lua>().tableName]["moveLeft"](_playerID, timeSinceLastFrame);
            //_luaMgr.executeFunction("moveLeft", _playerID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::Up, [this](const sf::Event &, double timeSinceLastFrame) {
            _luaMgr[_ettMgr[_playerID].getComponent<rtc::Lua>().tableName]["moveUp"](_playerID, timeSinceLastFrame);
            //_luaMgr.executeFunction("moveUp", _playerID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::Down, [this](const sf::Event &, double timeSinceLastFrame) {
            _luaMgr[_ettMgr[_playerID].getComponent<rtc::Lua>().tableName]["moveDown"](_playerID, timeSinceLastFrame);
            //_luaMgr.executeFunction("moveDown", _playerID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::SpaceShoot, [this](const sf::Event &, [[maybe_unused]] double timeSinceLastFrame) {
            const rtc::BoundingBox &boundingBox = this->_ettMgr[_playerID].getComponent<rtc::BoundingBox>();
            using namespace std::chrono_literals;
            auto res = std::chrono::steady_clock::now();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                std::chrono::duration_cast<std::chrono::milliseconds>(res - _lastShoot) > 200ms) {
                auto id = GameFactory::createBullet(_playerID, _textures.get(demo::Sprite::Bullet), boundingBox.AABB,
                                                    Configuration::SoundEffect::Laser4);
                this->_quadTree.insert(id);
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

    void DemoScene::_registerAdditionalLuaFunctions() noexcept
    {
        _luaMgr["quadMove"] = [this](Entity::ID id) { this->_quadTree.move(id); };
        _luaMgr["quadRemove"] = [this](Entity::ID id) { this->_quadTree.remove(id); };

    }

    void DemoScene::receive(const gutils::evt::SetPvBoss &evt) noexcept {
        auto& stat = evt.stat;
        stat.hpBar = static_cast<CEGUI::ProgressBar *>(&_gui[ui::UIWidgets::PVBoss]);
        stat.hpBar->setProgress(1);
    }
}