//
// Created by milerius on 22/12/17.
//

#ifndef RTYPE_DEMOSCENE_HPP
#define RTYPE_DEMOSCENE_HPP

#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/InGameConfig.hpp>
#include <rtype/gutils/base/AScene.hpp>

namespace rtype
{
    class DemoScene final : public gutils::AScene
    {
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
        void __configure() noexcept;
        bool __setGUI() noexcept;

    private:
        using UIGUI = BaseGUI<cfg::play::nbWidgets>;
        UIGUI _gui;
    };
}

#endif //RTYPE_DEMOSCENE_HPP
