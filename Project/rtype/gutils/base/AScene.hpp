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
        //! Constructors

        explicit AScene(gutils::EventManager &evtMgr, std::string_view sceneName, sf::RenderWindow& win) noexcept :
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
         * @note
         * The resume function allows you to resume the scene after a pause for example.
         */
        virtual void resume() noexcept = 0;

        /**
         * @note
         * The pause function does the reverse work of the resume function, it allows you to pause the scene.
         */
        virtual void pause() noexcept = 0;

        /**
         * @note
         * The draw function allows you to display on your stage, this is where you have to display your drawables.
         * This is the last function of the pipeline that is called.

         */
        virtual void draw() noexcept = 0;

        /**
         * @note
         * The function leave is to call when you leave the scene, it is generally here that you will clean your scene.
         */
        virtual void leave() noexcept = 0;

        /**
         * @note
         * The update function is one of the most important.
         * It is automatically called by the scene manager and allows you to update the entities of your scene.
         */
        virtual void update(double timeSinceLastFrame) noexcept = 0;

        /**
         * @note
         * The KeyPressed function is called when the user presses a key on his keyboard, it is blocking,
         * if you want to make non-blocking input, you will have to go through the update function.
         */
        virtual bool keyPressed(const evt::KeyPressed &evt) noexcept = 0;

        /**
         * @note
         * The keyReleased function is called when the user releases the key from his keyboard.
         */
        virtual bool keyReleased(const evt::KeyReleased &evt) noexcept = 0;

        /**
         * @note
         * The mouseMoved function is called when the user moves their mouse
         */
        virtual bool mouseMoved(const evt::MouseMoved &evt) noexcept = 0;

        /**
         * @note
         * The mousePressed function is called when the user presses one of the mouse buttons.
         */
        virtual bool mousePressed(const evt::MouseButtonPressed &evt) noexcept = 0;

        /**
         * @note
         * The mouseReleased function is called when the user releases one of the mouse buttons.
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
        //! Protected member variables

        /**
         * @note You can use the event manager to subscribe or issue an event in your own game scenes.
         */
        gutils::EventManager &_evtMgr;

        /**
         * @note used for logging.
         */
        std::string _sceneName;

        /**
         * @note internal logger for a scene.
         */
        logging::Logger _log{_sceneName, logging::Info};

        /**
         * @note internal use for scenes which needs to draw something.
         */
        sf::RenderWindow &_win;
    };
}

#endif //GUTILS_ASCENE_HPP