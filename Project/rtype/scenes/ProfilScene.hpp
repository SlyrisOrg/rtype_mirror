//
// Created by milerius on 10/11/17.
//

#ifndef RTYPE_PROFILSCENE_HPP
#define RTYPE_PROFILSCENE_HPP

#include <utility>
#include <utils/Enums.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/scenes/private/ForwardEnumProfil.hpp>
#include <rtype/scenes/private/GameSettings.hpp>

namespace rtype
{
    class ProfilScene final : public gutils::AScene
    {
    public:
        template <typename ...Args>
        ProfilScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {
            _evtMgr.subscribe<gutils::evt::ResetText>(*this);
        }

    private:
        using ProfilGui = BaseGUI<cfg::profil::nbWidgets>;
        using ProfilEnumT = ProfilWidgets::EnumType;

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
        void receive([[maybe_unused]] const gutils::evt::ResetText &evt) noexcept;

    private:
        //! CEGUI Callbacks
        void __resume() noexcept;
        void __leaveGame() noexcept;
        void __logOut() noexcept;
        void __optionsWindow() noexcept;
        void __closeOptionsWindow() noexcept;
        void __subscribeEvents() noexcept;
        void __setText() noexcept;
        bool __setGUI() noexcept;
        void __setData() noexcept;
        void __configure() noexcept;
        void __cancelMainMenuWindow() noexcept;
        void __checkResume(const sf::Keyboard::Key &key) noexcept;
        void __mainMenuWindow() noexcept;
        void __goingToQueue() noexcept;
        void __getData() const noexcept;

    private:
        ProfilGui _gui;
        GameSettings _settings{_gui, _evtMgr};
        sf::Sprite _bg;
        rtype::MultiAnim _anims;
        bool _pause{false};
    };
}

#endif //RTYPE_PROFILSCENE_HPP
