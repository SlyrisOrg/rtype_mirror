//
// Created by milerius on 22/12/17.
//

#ifndef RTYPE_DEMOSCENE_HPP
#define RTYPE_DEMOSCENE_HPP

#include <random>
#include <unordered_map>
#include <rapidjson/istreamwrapper.h>
#include <utils/Enums.hpp>
#include <rtype/entity/GameFactory.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/InGameConfig.hpp>
#include <rtype/config/PlayerConfig.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/utils/Action.hpp>
#include <rtype/utils/QuadTree.hpp>

namespace rtype
{
    namespace demo
    {
        ENUM(Sprite,
             BheetLv1AttackTopDown,
             BheetLv1AttackTurnUp,
             BheetLv1AttackTurnDown,
             BheetLv1AttackRedressUp,
             BheetLv1AttackRedressDown,
             BheetLv1AttackRollUp,
             BheetLv1AttackRollDown,
             Bullet,
             Flare01,
             Flare02,
             Flare03,
             Flare04);

        using SpriteT = demo::Sprite::EnumType;

        class StarfieldSystem
        {
        public:
            StarfieldSystem(sf::RenderWindow &win, EntityManager &ettMgr,
                            sfutils::ResourceManager<sf::Texture, SpriteT> &textures,
                            gutils::EventManager &evtMgr) noexcept :
                _win(win),
                _ettMgr(ettMgr),
                _textures(textures),
                _evtMgr(evtMgr)
            {
            };

        public:
            struct Star
            {
                unsigned int nbStars;
                float scale;
                demo::SpriteT sprite;
            };

            std::vector<Star> __parseConfigInside(const rapidjson::Document &document) noexcept
            {
                std::vector<Star> stars;
                const auto &starfieldCFG = document["starfield"];
                const auto &speedCFG = starfieldCFG.GetObject()["speedDistribution"];
                const auto &posXCFG = starfieldCFG.GetObject()["posXDistribution"];
                const auto &posYCFG = starfieldCFG.GetObject()["posYDistribution"];
                const auto &starCFG = starfieldCFG.GetObject()["stars"];
                _speedDistribution.param(std::uniform_real_distribution<float>(speedCFG.GetArray()[0].GetFloat(),
                                                                               speedCFG.GetArray()[1].GetFloat()).param());
                _distX.param(std::uniform_real_distribution<float>(posXCFG.GetArray()[0].GetFloat(),
                                                                   posXCFG.GetArray()[1].GetFloat()).param());
                _distY.param(std::uniform_real_distribution<float>(posYCFG.GetArray()[0].GetFloat(),
                                                                   posYCFG.GetArray()[1].GetFloat()).param());
                for (const auto &v : starCFG.GetArray()) {
                    Star star;
                    std::string_view spriteName = v.GetObject()["texture"].GetString();
                    Sprite spr = spriteName;
                    star.sprite = spr;
                    star.nbStars = v.GetObject()["nbStars"].GetUint();
                    star.scale = v.GetObject()["scale"].GetFloat();
                    stars.push_back(std::move(star));
                }
                return stars;
            }

            std::vector<Star> __parseConfigStarfield() noexcept
            {
                using namespace std::string_literals;
                try {
                    auto path = cfg::configPath + "starfield/starfield.json"s;
                    rapidjson::Document doc;
                    std::ifstream ifs(path);
                    rapidjson::IStreamWrapper isw(ifs);
                    doc.ParseStream(isw);
                    _log(lg::Info) << "Parsing file: " << path << std::endl;
                    if (doc.HasParseError()) {
                        _log(lg::Error) << doc.GetParseError() << " " << "Offset : " << doc.GetErrorOffset()
                                        << std::endl;
                        throw std::runtime_error("Parse file -> "s + path + " failed."s);
                    }
                    return __parseConfigInside(doc);
                }
                catch (const std::exception &error) {
                    _log(lg::Error) << error.what() << std::endl;
                    _evtMgr.emit<gutils::evt::GameShutdown>();
                }
                return {};
            }

            void configure() noexcept
            {
                std::vector<Star> stars = __parseConfigStarfield();
                float speed{0.0f};

                for (auto[nbStars, scale, sprite] : stars) {
                    for (unsigned int i = 0; i < nbStars; ++i) {
                        speed = this->_speedDistribution(this->_mt);
                        GameFactory::createStar(_textures.get(sprite), sf::Vector2f(_distX(_mt), _distY(_mt)),
                                                speed, scale, true, static_cast<unsigned int>(sprite));
                    }
                }
            }

            void update(double timeSinceLastFrame) noexcept
            {
                _ettMgr.for_each<rtc::Star>([this, &timeSinceLastFrame](rtype::Entity &ett) {
                    sf::Sprite &sprite = ett.getComponent<rtc::Sprite>().sprite;
                    if (sprite.getPosition().x < 0 && ett.hasComponent<rtc::Speed>()) {
                        ett.mark();
                        auto[star, speed, sprite] = ett.getComponents<rtc::Star, rtc::Speed, rtc::Sprite>();
                        GameFactory::createStar(_textures.get(static_cast<demo::SpriteT>(star.textureID)),
                                                sf::Vector2f(1920, _distY(_mt)),
                                                speed.speed, sprite.sprite.getScale().x, true, star.textureID);
                    } else {
                        if (ett.hasComponent<rtc::Speed>()) {

                            sprite.setPosition(
                                sprite.getPosition().x - (ett.getComponent<rtc::Speed>().speed * timeSinceLastFrame),
                                sprite.getPosition().y);
                        }
                    }
                });
            }

        private:
            sf::RenderWindow &_win;
            EntityManager &_ettMgr;
            sfutils::ResourceManager<sf::Texture, SpriteT> &_textures;
            gutils::EventManager &_evtMgr;
            logging::Logger _log{"DemoAnimationSystem", logging::Debug};

            std::random_device _rd;
            std::mt19937 _mt{_rd()};
            std::uniform_real_distribution<float> _distX{0.f, 1920.f};
            std::uniform_real_distribution<float> _distY{0.f, 1080.f};
            std::uniform_real_distribution<float> _speedDistribution{10.f, 100.f};
        };

        class AnimationSystem
        {
        public:
            ENUM(Animation,
                 BheetLv1AttackTopDown,
                 BheetLv1AttackTurnUp,
                 BheetLv1AttackTurnDown,
                 BheetLv1AttackRedressUp,
                 BheetLv1AttackRedressDown,
                 BheetLv1AttackRollUp,
                 BheetLv1AttackRollDown
            );

        private:
            using AnimT = Animation::EnumType;

        public:
            AnimationSystem(sf::RenderWindow &win, EntityManager &ettMgr, bool &debugMode,
                            sfutils::ResourceManager<sf::Texture, demo::SpriteT> &res,
                            const QuadTree<EntityManager> &quadTree) noexcept :
                _win(win), _ettMgr(ettMgr), _debugMode(debugMode), _textures(res), _quadTree(quadTree)
            {
            }

            void clear() noexcept
            {
                _animations.clear();
            }

            const sfutils::Animation &get(AnimT anim) const
            {
                return _animations.get(anim);
            }

            sfutils::Animation &get(AnimT anim)
            {
                return _animations.get(anim);
            }

            void update(double timeSinceLastFrame) noexcept
            {
                sf::Time time = sf::seconds(static_cast<float>(timeSinceLastFrame));
                _ettMgr.for_each<rtc::Animation, rtc::BoundingBox>([&time](rtype::Entity &ett) {
                    sfutils::AnimatedSprite &anim = ett.getComponent<rtc::Animation>().anim;
                    anim.update(time);
                });
                auto[movement, animation] = _ettMgr[_playerID].getComponents<rtc::Movement, rtc::Animation>();
                if (animation.anim.getStatus() == sfutils::AnimatedSprite::Stopped &&
                    (static_cast<AnimT>(animation.currentAnim) == Animation::BheetLv1AttackRedressUp ||
                     (static_cast<AnimT>(animation.currentAnim) == Animation::BheetLv1AttackRedressDown))) {
                    movement.dir = rtc::Direction::None;
                    animation.anim.setAnimation(&_animations.get(Animation::BheetLv1AttackTopDown));
                    animation.currentAnim = Animation::BheetLv1AttackTopDown;
                }
            }

            void draw() noexcept
            {
                _ettMgr.for_each<rtc::Animation>([this](rtype::Entity &ett) {
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
                });
            }

            void loadAnimation(const Sprite &sprite, const Animation &anim,
                               [[maybe_unused]] sf::Vector2<unsigned int> &&sprInfo,
                               sf::Vector3<unsigned int> &&sheetInfo)
            {
                __initFrames(anim, sprite, sheetInfo.y, sheetInfo.x, sheetInfo.z);
            }

            void setPlayerID(Entity::ID playerID) noexcept
            {
                _playerID = playerID;
            }

            void changePlayerAnim() noexcept
            {
                auto[movement, animation, player] = _ettMgr[_playerID].getComponents<rtc::Movement, rtc::Animation,
                    rtc::Player>();
                rtc::Direction dir = movement.dir;
                auto curAnim = static_cast<AnimT>(animation.currentAnim);
                size_t currentFrame = animation.anim.getCurrentFrame();

                auto animChange = [this, &currentFrame, &animation](int currentAnim) {
                    animation.currentAnim = currentAnim;
                    animation.anim.setAnimation(&_animations.get(static_cast<AnimT>(animation.currentAnim)),
                                                animation.anim.getAnimation()->size() - currentFrame);
                    animation.anim.play();
                };
                switch (dir) {
                    case rtc::Direction::None:
                        if (curAnim == Animation::BheetLv1AttackTurnUp) {
                            _log(lg::Debug) << "Redress up" << std::endl;
                            animChange(player.map[curAnim].second);
                        } else if (curAnim == Animation::BheetLv1AttackTurnDown) {
                            _log(lg::Debug) << "Redress Down" << std::endl;
                            animChange(player.map[curAnim].first);
                        }
                        break;
                    case rtc::Direction::North:
                        animChange(player.map[curAnim].first);
                        break;
                    case rtc::Direction::South:
                        animChange(player.map[curAnim].second);
                        break;
                    default:
                        break;
                }
            }

        private:
            void __initFrames(const Animation &anim,
                              const Sprite &sprite,
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
                _log(lg::Debug) << "Animation: " << anim.toString() << " information : nbColumns -> " << nbColumns
                                << ", nbLines" << nbLines << ", nbAnims: " << nbAnims << std::endl;
                _log(lg::Debug) << "Loading Animation " << anim.toString() << " successfully loaded." << std::endl;
            }

        private:
            sf::RenderWindow &_win;
            EntityManager &_ettMgr;
            bool &_debugMode;
            logging::Logger _log{"DemoAnimationSystem", logging::Debug};
            sfutils::ResourceManager<sf::Texture, SpriteT> &_textures;
            sfutils::ResourceManager<sfutils::Animation, AnimT> _animations;
            Entity::ID _playerID;
            const QuadTree<EntityManager> &_quadTree;
        };
    }

    class DemoScene final : public gutils::AScene, public rtype::ActionTarget<int>
    {
    public:
        template <typename ...Args>
        DemoScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...),
                                             ActionTarget(cfg::player::playerInputs)
        {
        }

        ~DemoScene() noexcept = default;

    public:
        void enter() noexcept final;
        void resume() noexcept final;
        void pause() noexcept final;
        void draw() noexcept final;
        void leave() noexcept final;
        void update(double timeSinceLastFrame) noexcept final;
        bool keyPressed(const gutils::evt::KeyPressed &evt) noexcept final;
        bool keyReleased(const gutils::evt::KeyReleased &evt) noexcept final;
        bool mouseMoved(const gutils::evt::MouseMoved &evt) noexcept final;
        bool mousePressed(const gutils::evt::MouseButtonPressed &evt) noexcept final;
        bool mouseReleased(const gutils::evt::MouseButtonReleased &evt) noexcept final;

    private:
        using UIGUI = BaseGUI<cfg::play::nbWidgets>;
        using AnimDemo = demo::AnimationSystem::Animation::EnumType;

    private:
        void __configure() noexcept;
        bool __setGUI() noexcept;
        void __parseConfig(const std::string &faction);
        void __parseConfigInside(const rapidjson::Document &doc, const std::string &string);
        void __loadSprite(const demo::Sprite &val);
        void __createGameObjects() noexcept;
        void __bulletSystem(double timeSinceLastFrame) noexcept;
        void __loadBulletSprite() noexcept;
        void __loadFlareSprite() noexcept;
        void __setPlayerCallbacks() noexcept;
        void __unbindPlayerCallbacks() noexcept;
    private:
        UIGUI _gui;
        sfutils::ResourceManager<sf::Texture, demo::SpriteT> _textures;
        std::unordered_map<std::string, sf::IntRect> _boundingBoxFactions;
        bool _debugMode{false};
        EntityManager _ettMgr;
        QuadTree<EntityManager> _quadTree{sf::FloatRect(0.f, 0.f, 1920, 1080), _ettMgr};
        demo::AnimationSystem _animSystem{_win, _ettMgr, _debugMode, _textures, _quadTree};
        demo::StarfieldSystem _starfieldSystem{_win, _ettMgr, _textures, _evtMgr};
        std::chrono::time_point<std::chrono::steady_clock> _lastShoot;
        Entity::ID _playerID;
    };
}

#endif //RTYPE_DEMOSCENE_HPP
