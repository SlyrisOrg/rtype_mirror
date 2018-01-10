//
// Created by milerius on 22/12/17.
//

#ifndef RTYPE_DEMOSCENE_HPP
#define RTYPE_DEMOSCENE_HPP

#include <utils/Enums.hpp>
#include <rtype/entity/GameFactory.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/InGameConfig.hpp>
#include <rtype/gutils/base/AScene.hpp>

namespace rtype
{
    class DemoScene final : public gutils::AScene
    {
    public:
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

        ENUM(Animation,
             BheetLv1AttackTopDown,
             BheetLv1AttackTurnUp,
             BheetLv1AttackTurnDown,
             BheetLv1AttackRedressUp,
             BheetLv1AttackRedressDown,
             BheetLv1AttackRollUp,
             BheetLv1AttackRollDown
        );

    public:
        template <typename ...Args>
        DemoScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
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
        using SpriteDemo = Sprite::EnumType;
        using AnimDemo = Animation::EnumType;

    private:
        void __configure() noexcept;
        bool __setGUI() noexcept;
        void __parseConfig(const std::string &faction);
        void __parseConfigInside(const rapidjson::Document &doc, const std::string &string);
        void __loadSprite(const Sprite &val);
        void __loadAnimation(const Sprite &sprite, const Animation &anim,
                             sf::Vector2<unsigned int> &&sprInfo,
                             sf::Vector3<unsigned int> &&sheetInfo);
        void __initFrames(const Animation &anim,
                          const Sprite &sprite,
                          unsigned int nbColumns,
                          unsigned int nbLines,
                          unsigned int nbAnims);
        void __createGameObjects() noexcept;
        void __animationSystem(const sf::Time &time) noexcept;
        void __inputSystem(double timeSinceLastFrame) noexcept;
        void __bulletSystem(double timeSinceLastFrame) noexcept;
        void __loadBulletSprite() noexcept;

    private:
        UIGUI _gui;
        sfutils::ResourceManager<sf::Texture, SpriteDemo> _textures;
        sfutils::ResourceManager<sfutils::Animation, AnimDemo> _animations;
        std::unordered_map<std::string, sf::IntRect> _boundingBoxFactions;
        EntityManager _ettMgr;
        bool _debugMode{false};
        std::chrono::time_point<std::chrono::steady_clock> _lastShoot;
    };
}

#endif //RTYPE_DEMOSCENE_HPP
