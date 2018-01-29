//
// Created by milerius on 24/01/18.
//

#include <rtype/gui/GUIManager.hpp>
#include <rtype/scenes/GameScene.hpp>
#include <rapidjson/istreamwrapper.h>
#include <rtype/config/ProfilConfig.hpp>
#include <rtype/entity/GameFactory.hpp>
#include <rtype/config/PlayerConfig.hpp>

namespace rtype
{
    void GameScene::enter() noexcept
    {
        _configure();
        showEnterScene();
    }

    void GameScene::resume() noexcept
    {
        showResumeScene();
    }

    void GameScene::pause() noexcept
    {
        showPauseScene();
    }

    void GameScene::draw() noexcept
    {
        if (this->_debugMode) {
            _win.draw(_quadTree.getRoot()._shapeDebug);
        }

        auto spriteDrawer = [this](rtype::Entity &ett) {
            if (this->_debugMode && ett.hasComponent<rtc::BoundingBox>()) {
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
                try {
                    _win.draw(_quadTree.getNode(ett.getID())->_shapeDebug);
                }
                catch (const std::out_of_range &error) {
                    _log(lg::Error) << error.what() << std::endl;
                }
            }
            this->_win.draw(ett.getComponent<rtc::Sprite>().sprite);
        };

        auto animDrawer = [this](rtype::Entity &ett) {
            if (this->_debugMode) {
                _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
                try {
                    _win.draw(_quadTree.getNode(ett.getID())->_shapeDebug);
                }
                catch (const std::out_of_range &error) {
                    _log(lg::Error) << error.what() << std::endl;
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

    void GameScene::leave() noexcept
    {
        showLeavingScene();
        _log(lg::Debug) << "NbPacketsReceive: " << nbPacketsReceive << std::endl;
        _log(lg::Debug) << "NbPacketsSend: " << nbPacketsSend << std::endl;
        _boundingBoxFactions.clear();
        _ettMgr.clear();
        _animations.clear();
        _textures.clear();
        _quadTree.clear();
        _fieldSystem.clear();
        _ioThread.reset();
    }

    void GameScene::update(double timeSinceLastFrame) noexcept
    {
        ActionTarget::processEvents(timeSinceLastFrame);
        auto show = [](auto &&v, auto &&log) {
            using Decayed = std::decay_t<decltype(v)>;
            if constexpr (!std::is_same_v<std::monostate, Decayed>) {
                log(lg::Debug) << "Got a packet of type " << Decayed::className() << std::endl;
            }
        };
        net::details::GamePackets packet;
        while (_ioThread->queue().pop(packet)) {
            auto visitor = meta::makeVisitor([this, &show](game::Welcome &welcomePacket) {
                show(welcomePacket, _log);
                game::Authenticate auth{cfg::game::tokenSession};
                _ioThread->sendPacket(auth);
            }, [this, &show](game::CreatePlayer &createPacket) {
                show(createPacket, _log);
                _createPlayer(createPacket);
            }, [this, &show](game::SetPosition &positionPacket) {
                show(positionPacket, _log);
                nbPacketsReceive++;
                _moveEntity(positionPacket.pos, positionPacket.ettName);
            }, [this, &show](auto &&v) {
                show(v, _log);
            });
            std::visit(visitor, packet);
        }

        _fieldSystem.update(timeSinceLastFrame);
        sf::Time time = sf::seconds(static_cast<float>(timeSinceLastFrame));
        _ettMgr.for_each<rtc::Animation, rtc::BoundingBox>([&time](rtype::Entity &ett) {
            sfutils::AnimatedSprite &anim = ett.getComponent<rtc::Animation>().anim;
            anim.update(time);
        });
        _ettMgr.sweepEntities();
    }

    bool GameScene::keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept
    {
        //! Temporary
        if (evt.key == sf::Keyboard::F2) {
            _debugMode = !_debugMode;
        } else if (evt.key == sf::Keyboard::Escape) {
            _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
        }
        return false;
    }

    bool GameScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool GameScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool GameScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool GameScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void GameScene::receive([[maybe_unused]] const gutils::evt::GameHostPort &hostPort) noexcept
    {
        _remoteGamePort = hostPort.hostGamePort;
        _log(lg::Info) << "Using " << _remoteGamePort << " as remote game port." << std::endl;
    }

    void GameScene::_configure()
    {
        auto start = _setGUI();
        if (start) {
            _luaMgr.loadAll();
            _luaMgr.loadScript("player.lua");
            GameFactory::setEntityManager(&_ettMgr);
            GameFactory::setLuaManager(&_luaMgr);
            GameFactory::setQuadTree(&_quadTree);
            _loadAllSprites();
            _configureAnimations("Bheet");
            _configureAnimations("Kooy");
            _configureAnimations("Maul");
            _fieldSystem.configure();
            _registerAdditionalLuaFunctions();
            _configureNetwork();
        }
    }

    bool GameScene::_setGUI() noexcept
    {
        using namespace cfg::play;
        return GUIManager::setGUI<ui::UIWidgets, nbWidgets>(UILayout, _gui, _log);
    }

    void GameScene::_configureNetwork()
    {
        tcp::endpoint endpoint{asio::ip::address::from_string("192.168.0.31"), _remoteGamePort};
        _ioThread = std::make_unique<net::ClientIOThread>(endpoint);
        _ioThread->run();

        // TODO: Connection
    }

    void GameScene::_configureAnimations(const std::string &faction)
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
            _parseConfigInside(doc, faction);
        }
        catch (const std::exception &error) {
            _log(lg::Error) << error.what() << std::endl;
            _evtMgr.emit<gutils::evt::GameShutdown>();
        }
    }

    void GameScene::_parseConfigInside(const rapidjson::Document &doc, const std::string &faction)
    {
        const auto &animCFG = doc["Animation"];
        const auto &boxCFG = doc["Box"];
        for (const auto &v : animCFG.GetArray()) {
            std::string_view animName = v.GetObject()["name"].GetString();
            ig::Sprite spriteValue = animName;
            _loadSprite(spriteValue);
            ig::Anim animValue = animName;
            sf::Vector2u spriteInfo{v.GetObject()["spriteInfo"].GetObject()["width"].GetUint(),
                                    v.GetObject()["spriteInfo"].GetObject()["height"].GetUint()};
            sf::Vector3<unsigned int> sheetInfo{v.GetObject()["sheetInfo"].GetObject()["rows"].GetUint(),
                                                v.GetObject()["sheetInfo"].GetObject()["column"].GetUint(),
                                                v.GetObject()["sheetInfo"].GetObject()["nbSprite"].GetUint()};
            _animLoad(spriteValue, animValue, std::move(spriteInfo), std::move(sheetInfo));
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

    void GameScene::_animLoad(const ig::Sprite &sprite, const ig::Anim &anim,
                              [[maybe_unused]] sf::Vector2<unsigned int> &&sprInfo,
                              sf::Vector3<unsigned int> &&sheetInfo)
    {
        _initFrames(anim, sprite, sheetInfo.y, sheetInfo.x, sheetInfo.z);
    }

    void GameScene::_loadSprite(const ig::Sprite &val)
    {
        auto str = cfg::spritePath + val.toString() + ".png";
        _textures.load(val, str);
        _log(lg::Debug) << "Loading Image " << val.toString() + ".png" << " successfully loaded." << std::endl;
    }

    void GameScene::_initFrames(const ig::Anim &anim, const ig::Sprite &sprite, unsigned int nbColumns,
                                unsigned int nbLines, unsigned int nbAnims)
    {
        auto &text = _animations.load(anim, &_textures.get(sprite));
        auto end = nbLines - 1;
        for (unsigned int i = 0; i < end; i++) {
            text.addFramesLine(nbColumns, nbLines, i, nbColumns);
        }
        auto animLastLine = (nbAnims - ((nbLines - 1) * nbColumns));
        text.addFramesLine(animLastLine, nbLines, nbLines - 1, nbColumns);
        _log(lg::Debug) << "Animation: " << anim.toString() << " information : nbColumns -> " << nbColumns
                        << ", nbLines" << nbLines << ", nbAnims: " << nbAnims << std::endl;
        _log(lg::Debug) << "Loading Animation " << anim.toString() << " successfully loaded." << std::endl;
    }

    void GameScene::_createPlayer(const game::CreatePlayer &createPacket) noexcept
    {
        static std::vector<game::CreatePlayer > _players;
        _players.push_back(createPacket);
        if (_players.size() == cfg::player::mode + 1) {
            for (auto &&cur : _players) {
                const auto &map = _factionTransitionMap[cur.factionName];
                ig::AnimT val;
                if (cur.factionName == "Bheet")
                    val = ig::Anim::BheetLv1AttackTopDown;
                else if (cur.factionName == "Kooy")
                    val = ig::Anim::KooyLv1AttackTopDown;
                else
                    val = ig::Anim::MaulLv1TankTopDown;
                Entity::ID id = GameFactory::createPlayerSpaceShip(map,
                                                               _animations.get(val),
                                                               _boundingBoxFactions[cur.factionName],
                                                               cur.pos,
                                                               rtype::components::Stat{1,1,1,700,1});
                _quadTree.insert(id);
                _log(lg::Debug) << "createNickname: " << cur.nickName << std::endl;
                _entities.emplace(cur.nickName, id);
                _ettMgr[id].getComponent<rtc::Animation>().currentAnim = val;
                if (cur.nickName == cfg::profil::nickname)
                    _setPlayerCallbacks();
            }
            _players.clear();
        }
    }

    void GameScene::_loadAllSprites()
    {
        _loadSprite(ig::Sprite::BluePlanet1);
        _loadSprite(ig::Sprite::BluePlanet2);
        _loadSprite(ig::Sprite::BluePlanet3);
        _loadSprite(ig::Sprite::BluePlanet4);
        _loadSprite(ig::Sprite::BluePlanet5);
        _loadSprite(ig::Sprite::BluePlanet6);
        _loadSprite(ig::Sprite::BluePlanet7);
        _loadSprite(ig::Sprite::BluePlanet8);
        _loadSprite(ig::Sprite::BluePlanet9);
        _loadSprite(ig::Sprite::BluePlanet10);
        _loadSprite(ig::Sprite::BluePlanet11);
        _loadSprite(ig::Sprite::BluePlanet12);
        _loadSprite(ig::Sprite::BluePlanet13);
        _loadSprite(ig::Sprite::BluePlanet14);
        _loadSprite(ig::Sprite::BluePlanet15);
        _loadSprite(ig::Sprite::BluePlanet16);
        _loadSprite(ig::Sprite::BluePlanet17);
        _loadSprite(ig::Sprite::BluePlanet18);
        _loadSprite(ig::Sprite::BluePlanet19);
        _loadSprite(ig::Sprite::RedPlanet1);
        _loadSprite(ig::Sprite::RedPlanet2);
        _loadSprite(ig::Sprite::RedPlanet3);
        _loadSprite(ig::Sprite::RedPlanet4);
        _loadSprite(ig::Sprite::RedPlanet5);
        _loadSprite(ig::Sprite::RedPlanet6);
        _loadSprite(ig::Sprite::RedPlanet7);
        _loadSprite(ig::Sprite::RedPlanet8);
        _loadSprite(ig::Sprite::RedPlanet9);
        _loadSprite(ig::Sprite::RedPlanet10);
        _loadSprite(ig::Sprite::RedPlanet11);
        _loadSprite(ig::Sprite::RedPlanet12);
        _loadSprite(ig::Sprite::RedPlanet13);
        _loadSprite(ig::Sprite::RedPlanet14);
        _loadSprite(ig::Sprite::RedPlanet15);
        _loadSprite(ig::Sprite::RedPlanet16);
        _loadSprite(ig::Sprite::RedPlanet17);
        _loadSprite(ig::Sprite::RedPlanet18);
        _loadSprite(ig::Sprite::RedPlanet19);
        _loadSprite(ig::Sprite::GreenPlanet1);
        _loadSprite(ig::Sprite::GreenPlanet2);
        _loadSprite(ig::Sprite::GreenPlanet3);
        _loadSprite(ig::Sprite::GreenPlanet4);
        _loadSprite(ig::Sprite::GreenPlanet5);
        _loadSprite(ig::Sprite::GreenPlanet6);
        _loadSprite(ig::Sprite::GreenPlanet7);
        _loadSprite(ig::Sprite::GreenPlanet8);
        _loadSprite(ig::Sprite::GreenPlanet9);
        _loadSprite(ig::Sprite::GreenPlanet10);
        _loadSprite(ig::Sprite::GreenPlanet11);
        _loadSprite(ig::Sprite::GreenPlanet12);
        _loadSprite(ig::Sprite::GreenPlanet13);
        _loadSprite(ig::Sprite::GreenPlanet14);
        _loadSprite(ig::Sprite::WhiteStar1);
        _loadSprite(ig::Sprite::WhiteStar2);
        _loadSprite(ig::Sprite::GreenStar1);
        _loadSprite(ig::Sprite::GreenStar2);
        _loadSprite(ig::Sprite::GreenStar3);
        _loadSprite(ig::Sprite::GreenStar4);
        _loadSprite(ig::Sprite::GreenStar5);
        _loadSprite(ig::Sprite::BlueStar1);
        _loadSprite(ig::Sprite::BlueStar2);
        _loadSprite(ig::Sprite::BlueStar3);
        _loadSprite(ig::Sprite::BlueStar4);
        _loadSprite(ig::Sprite::BlueStar5);
        _loadSprite(ig::Sprite::BlueStar6);
        _loadSprite(ig::Sprite::BlueStar7);
        _loadSprite(ig::Sprite::RedStar1);
        _loadSprite(ig::Sprite::RedStar2);
        _loadSprite(ig::Sprite::RedStar3);
        _loadSprite(ig::Sprite::RedStar4);
        _loadSprite(ig::Sprite::RedStar5);
        _loadSprite(ig::Sprite::RedStar6);
        _loadSprite(ig::Sprite::RedStar7);
        _loadSprite(ig::Sprite::GreenFog1);
        _loadSprite(ig::Sprite::GreenFog2);
        _loadSprite(ig::Sprite::RedFog1);
        _loadSprite(ig::Sprite::RedFog2);
        _loadSprite(ig::Sprite::BlueFog1);
        _loadSprite(ig::Sprite::BlueFog2);
        _loadSprite(ig::Sprite::BlueFog3);
        _loadSprite(ig::Sprite::Bullet);
    }

    void GameScene::_setPlayerCallbacks() noexcept
    {
        setKeyCallback(cfg::player::Right, [this](const sf::Event &, double timeSinceLastFrame) {
            this->_ioThread->sendPacket(game::Move{game::Move::Direction::Right, cfg::profil::nickname,
                                                   static_cast<float>(timeSinceLastFrame)});
            nbPacketsSend++;
            Entity::ID ID = _entities[cfg::profil::nickname];
            _luaMgr[_ettMgr[ID].getComponent<rtc::Lua>().tableName]["moveRight"](ID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::Left, [this](const sf::Event &, double timeSinceLastFrame) {
            this->_ioThread->sendPacket(game::Move{game::Move::Direction::Left,
                                                   cfg::profil::nickname,
                                                   static_cast<float>(timeSinceLastFrame)});
            nbPacketsSend++;
            Entity::ID ID = _entities[cfg::profil::nickname];
            _luaMgr[_ettMgr[ID].getComponent<rtc::Lua>().tableName]["moveLeft"](ID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::Up, [this](const sf::Event &, double timeSinceLastFrame) {
            this->_ioThread->sendPacket(game::Move{game::Move::Direction::Up,
                                                   cfg::profil::nickname,
                                                   static_cast<float>(timeSinceLastFrame)});
            nbPacketsSend++;
            Entity::ID ID = _entities[cfg::profil::nickname];
            _luaMgr[_ettMgr[ID].getComponent<rtc::Lua>().tableName]["moveUp"](ID, timeSinceLastFrame);
        });

        setKeyCallback(cfg::player::Down, [this](const sf::Event &, double timeSinceLastFrame) {
            this->_ioThread->sendPacket(game::Move{game::Move::Direction::Down,
                                                   cfg::profil::nickname,
                                                   static_cast<float>(timeSinceLastFrame)});
            nbPacketsSend++;
            Entity::ID ID = _entities[cfg::profil::nickname];
            _luaMgr[_ettMgr[ID].getComponent<rtc::Lua>().tableName]["moveDown"](ID, timeSinceLastFrame);
        });
    }

    void GameScene::_moveEntity(const sf::Vector2f &pos, const std::string &ettName)
    {
        auto[box, animation] = this->_ettMgr[_entities[ettName]].getComponents<rtc::BoundingBox, rtc::Animation>();
        sfutils::AnimatedSprite &anim = animation.anim;
        auto limitX = (cfg::game::width - box.AABB.width);
        if (box.AABB.left <= limitX) {
            box.setPosition(pos);
            anim.setPosition(pos.x - box.relativeAABB.left,
                             pos.y - box.relativeAABB.top);
            _quadTree.move(_entities[cfg::profil::nickname]);
        }
    }

    void GameScene::_registerAdditionalLuaFunctions() noexcept
    {
        _luaMgr["quadMove"] = [this](Entity::ID id) { this->_quadTree.move(id); };
    }
}