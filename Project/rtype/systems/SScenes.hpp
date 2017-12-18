//
// Created by roman sztergbaum on 17/09/2017.
//

#ifndef RTYPE_SSCENES_HPP
#define RTYPE_SSCENES_HPP

#include <rtype/scenes/EnumScenes.hpp>
#include <rtype/scenes/SceneManager.hpp>
#include <rtype/scenes/LoginScene.hpp>
#include <rtype/scenes/CreateScene.hpp>
#include <rtype/scenes/IntroScene.hpp>
#include <rtype/scenes/ProfilScene.hpp>
#include <rtype/scenes/LoadingScene.hpp>

namespace rtype
{
    class SScenes : public gutils::System<SScenes>
    {
    public:

        explicit SScenes(gutils::EventManager &evtMgr, sf::RenderWindow &win) noexcept :
            gutils::System<SScenes>(evtMgr, "Scenes"), _sceneMgr(evtMgr)
        {
            _sceneMgr.add<LoadingScene>(Scene::Loading, _evtMgr, "LoadingScene", win);
            _sceneMgr.add<IntroScene>(Scene::Intro, _evtMgr, "IntroScene", win);
            _sceneMgr.add<LoginScene>(Scene::Login, _evtMgr, "LoginScene", win);
            _sceneMgr.add<CreateScene>(Scene::Create, _evtMgr, "CreateScene", win);
            _sceneMgr.add<ProfilScene>(Scene::Profil, _evtMgr, "ProfilScene", win);
        }

        void update(double deltaTime) noexcept override
        {
            _sceneMgr.update(deltaTime);
        }

        void draw() noexcept override
        {
            _sceneMgr.draw();
        }

    private:
        SceneManager _sceneMgr;
    };
}

#endif //RTYPE_SSCENES_HPP