//
// Created by zouz on 21/12/17.
//

#ifndef RTYPE_LOBBYSCENE_HPP
#define RTYPE_LOBBYSCENE_HPP


#include <rtype/gutils/base/AScene.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/LobbyConfig.hpp>


namespace rtype
{
    class LobbyScene : public gutils::AScene
    {
    public:
        template <typename ... Args>
        LobbyScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {
        }

        ~LobbyScene() noexcept = default;

    private:
        using LobbyGui = BaseGUI<cfg::lobby::nbWidgets>;

    private:
        //! Private Member functions.
        void __configure() noexcept;
        bool __setGUI() noexcept;
        void __initTexture() noexcept;
        void __subscribeEvents() noexcept;
        void __goBacktoQueue() noexcept;
        void __resetUI() noexcept;

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
        LobbyGui _gui;
        sf::Sprite _bg;
        std::vector<sf::Sprite> _bordersBg;
    };
}

#endif //RTYPE_LOBBYSCENE_HPP
