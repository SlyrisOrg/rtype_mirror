//
// Created by milerius on 24/01/18.
//

#ifndef RTYPE_GAMESCENE_HPP
#define RTYPE_GAMESCENE_HPP

#include <rtype/gutils/base/AScene.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/netproto/ClientIOThread.hpp>
#include <rtype/config/InGameConfig.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/utils/QuadTree.hpp>
#include <rtype/scenes/private/ForwardGameSceneUtils.hpp>
#include <rtype/systems/GameFieldSystem.hpp>
#include <rtype/utils/Action.hpp>
#include <rtype/config/PlayerConfig.hpp>

namespace rtype
{
    class GameScene final : public gutils::AScene, public rtype::ActionTarget<int>
    {
    public:
        template <typename ...Args>
        GameScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...),
                                             ActionTarget(cfg::player::playerInputs)
        {
            _evtMgr.subscribe<gutils::evt::GameHostPort>(*this);
        }

        ~GameScene() noexcept final = default;

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
        void receive(const gutils::evt::GameHostPort &hostPort) noexcept;

    private:
        void _registerAdditionalLuaFunctions() noexcept;
        void _setPlayerCallbacks() noexcept;
        void _loadAllSprites();
        bool _setGUI() noexcept;
        void _configure();
        void _configureNetwork();
        void _configureAnimations(const std::string &faction);
        void _parseConfigInside(const rapidjson::Document &doc, const std::string &string);
        void _loadSprite(const ig::Sprite &val);
        void _animLoad(const ig::Sprite &sprite, const ig::Animation &anim,
                       [[maybe_unused]] sf::Vector2<unsigned int> &&sprInfo,
                       sf::Vector3<unsigned int> &&sheetInfo);
        void _initFrames(const ig::Animation &anim,
                         const ig::Sprite &sprite,
                         unsigned int nbColumns,
                         unsigned int nbLines,
                         unsigned int nbAnims);
        void _createPlayer(const game::CreatePlayer &createPacket) noexcept;
        void _moveEntity(const sf::Vector2f& pos, const std::string &ettName);
    private:
        using UIGUI = BaseGUI<cfg::play::nbWidgets>;
        UIGUI _gui;
        unsigned short _remoteGamePort{0};
        EntityManager _ettMgr;
        QuadTree<EntityManager> _quadTree{sf::FloatRect(0.f, 0.f, 1920, 1080), _ettMgr};

        std::unique_ptr<net::ClientIOThread> _ioThread{nullptr};
        std::unordered_map<int, std::pair<int, int>> _bheetTransMap
            {
                {
                    ig::Anim::BheetLv1AttackTopDown,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackTurnDown}
                },
                {
                    ig::Anim::BheetLv1AttackTurnUp,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackRedressUp}
                },
                {
                    ig::Anim::BheetLv1AttackTurnDown,
                    {ig::Anim::BheetLv1AttackRedressDown, ig::Anim::BheetLv1AttackTurnDown}
                },
                {
                    ig::Anim::BheetLv1AttackRedressUp,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackRedressUp}
                },
                {
                    ig::Anim::BheetLv1AttackRedressDown,
                    {ig::Anim::BheetLv1AttackRedressDown, ig::Anim::BheetLv1AttackTurnDown}
                }
            };

        std::unordered_map<int, std::pair<int, int>> _kooyTransMap
            {
                {
                    ig::Anim::BheetLv1AttackTopDown,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackTurnDown}
                },
                {
                    ig::Anim::BheetLv1AttackTurnUp,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackRedressUp}
                },
                {
                    ig::Anim::BheetLv1AttackTurnDown,
                    {ig::Anim::BheetLv1AttackRedressDown, ig::Anim::BheetLv1AttackTurnDown}
                },
                {
                    ig::Anim::BheetLv1AttackRedressUp,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackRedressUp}
                },
                {
                    ig::Anim::BheetLv1AttackRedressDown,
                    {ig::Anim::BheetLv1AttackRedressDown, ig::Anim::BheetLv1AttackTurnDown}
                }
            };

        std::unordered_map<int, std::pair<int, int>> _maulTransMap
            {
                {
                    ig::Anim::BheetLv1AttackTopDown,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackTurnDown}
                },
                {
                    ig::Anim::BheetLv1AttackTurnUp,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackRedressUp}
                },
                {
                    ig::Anim::BheetLv1AttackTurnDown,
                    {ig::Anim::BheetLv1AttackRedressDown, ig::Anim::BheetLv1AttackTurnDown}
                },
                {
                    ig::Anim::BheetLv1AttackRedressUp,
                    {ig::Anim::BheetLv1AttackTurnUp,      ig::Anim::BheetLv1AttackRedressUp}
                },
                {
                    ig::Anim::BheetLv1AttackRedressDown,
                    {ig::Anim::BheetLv1AttackRedressDown, ig::Anim::BheetLv1AttackTurnDown}
                }
            };

        using TransMap = std::unordered_map<int, std::pair<int, int>>;
        std::unordered_map<std::string, TransMap> _factionTransitionMap
            {
                {"Bheet", _bheetTransMap},
                {"Kooy",  _kooyTransMap},
                {"Maul",  _maulTransMap}
            };
        bool _debugMode{false};
        sfutils::ResourceManager<sf::Texture, ig::SpriteT> _textures;
        sfutils::ResourceManager<sfutils::Animation, ig::AnimT> _animations;
        std::unordered_map<std::string, sf::IntRect> _boundingBoxFactions;
        std::unordered_map<std::string, Entity::ID> _entities;
        ig::FieldSystem _fieldSystem{_ettMgr, _textures, _evtMgr};
        unsigned long nbPacketsSend{0};
        unsigned long nbPacketsReceive{0};
        rtype::lua::LuaManager _luaMgr{_ettMgr, fs::path(fs::current_path() / "assets/scripts/")};
    };
}

#endif //RTYPE_GAMESCENE_HPP
