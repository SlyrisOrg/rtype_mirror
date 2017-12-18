//
// Created by roman sztergbaum on 17/09/2017.
//

#ifndef RTYPE_GAMESCENE_HPP
#define RTYPE_GAMESCENE_HPP

#include <iomanip>
#include <SFML/Window/Keyboard.hpp>
#include <config/Config.hpp>
#include <utils/StringHelpers.hpp>
#include <rtype/config/LoginConfig.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/gui/GUIManager.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/scenes/EnumScenes.hpp>
#include <rtype/api/rtype-api.hpp>
#include <rtype/scenes/private/ForwardEnumLogin.hpp>

namespace rtype
{
    class LoginScene final : public gutils::AScene
    {
    public:
        template <typename ...Args>
        LoginScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {

        }

        ~LoginScene() noexcept = default;
        
    private:
        using LoginGui = BaseGUI<17>;

    private:
        //! Private member functions.
        void __subscribeEvents() noexcept;
        void __activeWindowAndTitle(CEGUI::Window &window, CEGUI::Window &title) noexcept;
        void __errorWindow(std::string textError) noexcept;
        void __cancelLeave() noexcept;
        void __leaveWindow() noexcept;
        void __disableStandardWidgets() noexcept;
        void __activeStandardWidgets() noexcept;
        inline void __setText() noexcept;
        inline void __setImage() noexcept;
        bool __setGUI() noexcept;
        void __configure() noexcept;
        void __checkResume(const sf::Keyboard::Key &key) noexcept;
        void __checkReturnWidgets(const sf::Keyboard::Key &key) noexcept;
        void __checkTabWidgets(const sf::Keyboard::Key &key) noexcept;
        void __noInternet(const std::string& msg) noexcept;
    private:
        //! CEGUI Callbacks.
        void __tabAccount() noexcept;
        void __tabPassword() noexcept;
        void __resume() noexcept;
        void __pause() noexcept;
        void __leaveGame() noexcept;
        void __login() noexcept;

    public:
        //! Public Member functions.
        void draw() noexcept final;
        void enter() noexcept final;
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
        bool _pause{false};
        LoginGui _gui;
    };
}

#endif //RTYPE_GAMESCENE_HPP