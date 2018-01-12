//
// Created by milerius on 22/12/17.
//

#ifndef RTYPE_DEMOSCENE_HPP
#define RTYPE_DEMOSCENE_HPP

#include <unordered_map>
#include <utils/Enums.hpp>
#include <rtype/entity/GameFactory.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/InGameConfig.hpp>
#include <rtype/config/PlayerConfig.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/utils/Action.hpp>

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
             Bullet
        );

        using SpriteT = demo::Sprite::EnumType;

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
                            sfutils::ResourceManager<sf::Texture, demo::SpriteT> &res) noexcept :
                _win(win), _ettMgr(ettMgr), _debugMode(debugMode), _textures(res)
            {
            }

            void clear() noexcept
            {
                _animations.clear();
            }

            sfutils::Animation *get(AnimT anim) const
            {
                return &_animations.get(anim);
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
                    if (this->_debugMode)
                        _win.draw(ett.getComponent<rtc::BoundingBox>().shapeDebug);
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
                    case components::None:
                        if (curAnim == Animation::BheetLv1AttackTurnUp) {
                            _log(lg::Debug) << "Redress up" << std::endl;
                            animChange(player.map[curAnim].second);
                        } else if (curAnim == Animation::BheetLv1AttackTurnDown) {
                            _log(lg::Debug) << "Redress Down" << std::endl;
                            animChange(player.map[curAnim].first);
                        }
                        break;
                    case components::North:
                        animChange(player.map[curAnim].first);
                        break;
                    case components::South:
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
        };
    }

    class DemoScene final : public gutils::AScene, public rtype::ActionTarget<int>
    {
    public:

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
        void __setPlayerCallbacks() noexcept;
        void __unbindPlayerCallbacks() noexcept;
    private:
        UIGUI _gui;
        sfutils::ResourceManager<sf::Texture, demo::SpriteT> _textures;
        std::unordered_map<std::string, sf::IntRect> _boundingBoxFactions;
        bool _debugMode{false};
        EntityManager _ettMgr;
        demo::AnimationSystem _animSystem{_win, _ettMgr, _debugMode, _textures};
        std::chrono::time_point<std::chrono::steady_clock> _lastShoot;
        Entity::ID _playerID;
    };
}

#endif //RTYPE_DEMOSCENE_HPP
