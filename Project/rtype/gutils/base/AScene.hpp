//
// Created by roman sztergbaum on 13/09/2017.
//

#ifndef GUTILS_ASCENE_HPP
#define GUTILS_ASCENE_HPP

#include <config/Config.hpp>
#include <utils/NonCopyable.hpp>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>

namespace gutils
{
    /**
     * @interface AScene
     * @brief the scene interface
     * @note all scenes should inherits from this interface
     */
    struct AScene : public gutils::Receiver<AScene>
    {
        explicit AScene(gutils::EventManager &evtMgr, std::string_view sceneName, sf::RenderWindow &win) noexcept :
            _evtMgr{evtMgr},
            _sceneName(sceneName),
            _win(win)
        {
        }

        //! Member Functions

        /**
         * @note
         * The enter function is the first function that is called by the sceneManager when launching a scene,
         * it is in this function that you have to configure your game scene.
         */
        virtual void enter() noexcept = 0;

        /**
         * @brief Resume the scene (after a pause for example)
         */
        virtual void resume() noexcept = 0;

        /**
         * @brief Pause the scene (opposite of resume)
         */
        virtual void pause() noexcept = 0;

        /**
         * @brief Display the entities, this is where you have to display your drawables.
         */
        virtual void draw() noexcept = 0;

        /**
         * @brief Leave the scene (this can be used to perform cleanup)
         */
        virtual void leave() noexcept = 0;

        /**
         * @brief Update the entities of a scene
         */
        virtual void update(double timeSinceLastFrame) noexcept = 0;

        /**
         * @brief Process when the user presses a key on his keyboard, doesn't support auto-repeat,
         * if you want to make auto-repeat input, you will have to go through the update function.
         */
        virtual bool keyPressed(const evt::KeyPressed &evt) noexcept = 0;

        /**
         * @brief Process when the user releases the key from his keyboard.
         */
        virtual bool keyReleased(const evt::KeyReleased &evt) noexcept = 0;

        /**
         * @brief Process when the user moves their mouse
         */
        virtual bool mouseMoved(const evt::MouseMoved &evt) noexcept = 0;

        /**
         * @brief Process when the user presses one of the mouse buttons.
         */
        virtual bool mousePressed(const evt::MouseButtonPressed &evt) noexcept = 0;

        /**
         * @brief Process when the user releases one of the mouse buttons.
         */
        virtual bool mouseReleased(const evt::MouseButtonReleased &evt) noexcept = 0;

    public:
        std::string_view getName() const noexcept
        {
            return _sceneName;
        }

    protected:
        //! Utility functions
        void showResumeScene() const noexcept
        {
            _log(logging::Info) << "Resuming " << _sceneName << "." << std::endl;
        }

        void showPauseScene() const noexcept
        {
            _log(logging::Info) << "Pausing " << _sceneName << "." << std::endl;
        }

        void showLeavingScene() const noexcept
        {
            _log(logging::Info) << "Leaving " << _sceneName << "." << std::endl;
        }

        void showEnterScene() const noexcept
        {
            _log(logging::Info) << "Entering " << _sceneName << " ..." << std::endl;
        }

    protected:
        gutils::EventManager &_evtMgr;
        std::string _sceneName;
        logging::Logger _log{_sceneName, logging::Info};
        sf::RenderWindow &_win;
    };
}

#endif //GUTILS_ASCENE_HPP