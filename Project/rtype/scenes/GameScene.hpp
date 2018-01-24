//
// Created by milerius on 24/01/18.
//

#ifndef RTYPE_GAMESCENE_HPP
#define RTYPE_GAMESCENE_HPP

#include <rtype/gutils/base/AScene.hpp>

namespace rtype
{
    class GameScene final : public gutils::AScene
    {
    public:
        template <typename ...Args>
        GameScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
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
        unsigned short _remoteGamePort{0};
    };
}

#endif //RTYPE_GAMESCENE_HPP
