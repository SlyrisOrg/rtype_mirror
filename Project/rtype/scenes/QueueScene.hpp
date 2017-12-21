//
// Created by zouz on 21/12/17.
//

#ifndef RTYPE_QUEUESCENE_HPP
#define RTYPE_QUEUESCENE_HPP

#include <rtype/gutils/base/AScene.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/config/QueueConfig.hpp>

namespace rtype
{

    class QueueScene final : public gutils::AScene
    {
    public:
        template <typename ...Args>
        QueueScene(Args &&...args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {
        }

        ~QueueScene() noexcept = default;

    private:
        using QueueGui = BaseGUI<cfg::queue::nbWidgets>;

    private:
        bool __setGUI() noexcept;
        void __configure() noexcept;
        void __subscribeEvents() noexcept;
        void __goBackToProfil() noexcept;
        void __resetUi() noexcept;
        void __queueMusicStart() noexcept;
        void __queueMusicStop() noexcept;
        void __initTextures() noexcept;

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
        QueueGui _gui;
        sf::Sprite _bg;
    };
}

#endif //RTYPE_QUEUESCENE_HPP
