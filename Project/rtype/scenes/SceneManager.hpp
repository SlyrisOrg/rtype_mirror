//
// Created by roman sztergbaum on 17/09/2017.
//

#ifndef RTYPE_SCENEMANAGER_HPP
#define RTYPE_SCENEMANAGER_HPP

#include <functional>
#include <memory>
#include <unordered_map>
#include <stack>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/scenes/EnumScenes.hpp>

namespace rtype
{
    class SceneManager : public gutils::Receiver<SceneManager>
    {
    public:
        explicit SceneManager(gutils::EventManager &evtMgr) : _evtMgr(evtMgr)
        {
            _evtMgr.subscribe<gutils::evt::KeyPressed>(*this);
            _evtMgr.subscribe<gutils::evt::KeyReleased>(*this);
            _evtMgr.subscribe<gutils::evt::MouseMoved>(*this);
            _evtMgr.subscribe<gutils::evt::MouseButtonReleased>(*this);
            _evtMgr.subscribe<gutils::evt::MouseButtonPressed>(*this);
            _evtMgr.subscribe<gutils::evt::EnterScene>(*this);
            _evtMgr.subscribe<gutils::evt::ChangeScene>(*this);
        }

        ~SceneManager() noexcept = default;

    public:
        //! Public member callback receiver

        void receive(const gutils::evt::ChangeScene &event) noexcept
        {
            popAllAndPushOneScene(get(event.sceneName));
        }

        void receive(const gutils::evt::EnterScene &event) noexcept
        {
            pushScene(get(event.scene));
        }

        void receive(const gutils::evt::KeyPressed &event) noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->keyPressed(event);
        }

        void receive(const gutils::evt::KeyReleased &event) noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->keyReleased(event);
        }

        void receive(const gutils::evt::MouseMoved &event) noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->mouseMoved(event);
        }

        void receive(const gutils::evt::MouseButtonPressed &event) noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->mousePressed(event);
        }

        void receive(const gutils::evt::MouseButtonReleased &event) noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->mouseReleased(event);
        }

    public:
        using ScenesMap = std::unordered_map<Scene, std::shared_ptr<gutils::AScene>>;

        void popScene() noexcept
        {
            if (!_scenes.empty())
                _scenes.pop();
            if (!_scenes.empty())
                _scenes.top()->resume();
        }

        void pushScene(gutils::AScene &scene) noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->pause();
            _scenes.push(&scene);
            scene.enter();
        }

        void changeScene(gutils::AScene &scene) noexcept
        {
            popScene();
            pushScene(scene);
        }

        void popAllAndPushOneScene(gutils::AScene &scene) noexcept
        {
            while (!_scenes.empty()) {
                _scenes.top()->leave();
                _scenes.pop();
            }
            pushScene(scene);
        }

        void update(double timeSinceLastFrame) const noexcept
        {
            if (!_scenes.empty())
                _scenes.top()->update(timeSinceLastFrame);
        }

        void draw() noexcept
        {
            if (!_scenes.empty()) {
                _scenes.top()->draw();
            }
        }

        const ScenesMap &get() const noexcept
        {
            return _scenesMap;
        }

        gutils::AScene &get(Scene scene) const noexcept
        {
            return *_scenesMap.at(scene);
        }

        template <typename T, typename ... Args>
        SceneManager &add(Scene scene, Args &&... args)
        {
            _scenesMap.emplace(scene, std::make_unique<T>(std::forward<Args>(args)...));
            return *this;
        }

    private:
        ScenesMap _scenesMap;
        std::stack<gutils::AScene *> _scenes;
        gutils::EventManager &_evtMgr;
    };
}

#endif //RTYPE_SCENEMANAGER_HPP
