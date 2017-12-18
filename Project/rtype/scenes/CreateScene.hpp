//
// Created by szterg_r on 22/10/2017.
//

#ifndef RTYPE_CREATESCENE_HPP
#define RTYPE_CREATESCENE_HPP

#include <deque>
#include <tuple>
#include <utility>
#include <functional>
#include <rtype/core/SafeCEGUI.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/gui/GUIManager.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/scenes/EnumScenes.hpp>
#include <rtype/api/rtype-api.hpp>
#include <rtype/utils/AnimatedSprite.hpp>
#include <rtype/scenes/private/ForwardEnumCreate.hpp>

namespace rtype
{
    class CreateScene final : public gutils::AScene
    {
    public:

        template <typename ...Args>
        CreateScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {
            _anims.curAnim.setPosition(static_cast<float>(cfg::game::width / 2 - 337),
                                       static_cast<float>(cfg::game::height / 2 - 237));
        }

        ~CreateScene() noexcept override = default;

    private:
        using CreateGui = BaseGUI<14>;
        using CreateFactionT = CreateWidgets::EnumType;
        using WidgetT = CreateWidgets::EnumType;

        struct Stats
        {
            unsigned int nb;
            unsigned int max;
            WidgetT widget;
        };

        using Attack = Stats;
        using Defense = Stats;
        using PV = Stats;
        using Support = Stats;

    private:
        void __subscribeEvents() noexcept;
        void __unselectOtherFactions(CreateFactionT factionA, CreateFactionT factionB) noexcept;
        void __changeSelectedFaction(CreateFactionT faction, CreateFactionT factionReset,
                                     CreateFactionT secondFactionReset) noexcept;
        void __firstFaction() noexcept;
        void __secondFaction() noexcept;
        void __thirdFaction() noexcept;
        void __createPlayer() noexcept;
        void __prepareLeave() noexcept;
        void __leaveGame() noexcept;
        void __setImage() noexcept;
        bool __setGUI() noexcept;
        void __setText() noexcept;
        void __setStats(CreateFactionT faction) noexcept;
        void __configure() noexcept;

    public:
        void enter()  noexcept final;
        void draw() noexcept final;
        void resume() noexcept final;
        void pause() noexcept final;
        void leave() noexcept final;
        void update(double timeSinceLastFrame) noexcept final;
        bool keyPressed(const gutils::evt::KeyPressed &evt) noexcept final;
        bool keyReleased(const gutils::evt::KeyReleased &evt) noexcept final;
        bool mouseMoved(const gutils::evt::MouseMoved &evt) noexcept final;
        bool mousePressed(const gutils::evt::MouseButtonPressed &evt) noexcept final;
        bool mouseReleased(const gutils::evt::MouseButtonReleased &evt) noexcept final;

    private:
        CreateGui _gui;
        sf::Sprite _bg;
        rtype::MultiAnim _anims;
        CreateFactionT __selectedFaction{CreateWidgets::Bheet};
        CreateFactionT __oldSelectedFaction{CreateWidgets::Bheet};
        std::unordered_map<CreateFactionT, std::tuple<PV, Attack, Defense, Support>> _stats
            {
                {CreateWidgets::Bheet, std::make_tuple(PV{1000, 10000, CreateWidgets::PV},
                                            Attack{200, 1000, CreateWidgets::Attack},
                                            Defense{70, 1000, CreateWidgets::Defense},
                                            Support{20, 1000, CreateWidgets::Support})},
                {CreateWidgets::Kooy,  std::make_tuple(PV{1550, 10000, CreateWidgets::PV},
                                            Attack{80, 1000, CreateWidgets::Attack},
                                            Defense{120, 1000, CreateWidgets::Defense},
                                            Support{150, 1000, CreateWidgets::Support})},
                {CreateWidgets::Maul,  std::make_tuple(PV{2100, 10000, CreateWidgets::PV},
                                            Attack{20, 1000, CreateWidgets::Attack},
                                            Defense{180, 1000, CreateWidgets::Defense},
                                            Support{80, 1000, CreateWidgets::Support})}
            };

        std::unordered_map<WidgetT, float> _progress
            {
                {
                    {CreateWidgets::PV, 0.f},
                    {CreateWidgets::Attack, 0.f},
                    {CreateWidgets::Defense, 0.f},
                    {CreateWidgets::Support, 0.f}
                }
            };
    };
}

#endif //RTYPE_CREATESCENE_HPP
