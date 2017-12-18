//
// Created by szterg_r on 24/10/2017.
//

#ifndef RTYPE_INTROSCENE_HPP
#define RTYPE_INTROSCENE_HPP

#include <utility>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/scenes/EnumScenes.hpp>

namespace rtype
{
    class IntroScene final : public gutils::AScene
    {
    public:
        template <typename ...Args>
        IntroScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {
        }

    public:
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
        void draw() noexcept final;
    };
}

#endif //RTYPE_INTROSCENE_HPP
