//
// Created by milerius on 14/12/17.
//

#ifndef RTYPE_LOADINGSCENE_HPP
#define RTYPE_LOADINGSCENE_HPP

#include <thread>
#include <rtype/gutils/base/AScene.hpp>
#include <rtype/scenes/private/ForwardEnumLoading.hpp>
#include <rtype/config/LoadingConfig.hpp>
#include <future>

namespace rtype
{
    class LoadingScene final : public gutils::AScene
    {
    public:
        template <typename ... Args>
        LoadingScene(Args &&... args) noexcept : gutils::AScene(std::forward<Args>(args)...)
        {

        }

    private:


        bool __setGUI() noexcept
        {
            using namespace cfg::loading;
            return GUIManager::setGUI<LW, cfg::loading::nbWidgets>(loadingLayout, _gui, _log);
        }

        void __configure() noexcept
        {
            auto start = __setGUI();
            if (start) {
                static_cast<CEGUI::ProgressBar&>(_gui[LW::LoadingProgressBar]).setProgress(_progress);
                _nbFiles = Configuration::Music::values().size()
                               + Configuration::Sprite::values().size()
                               + Configuration::Animation::values().size()
                               + Configuration::SoundEffect::values().size()
                               + Configuration::Video::values().size() + cfg::nbConfigs;
                _log(lg::Info) << _nbFiles << std::endl;
                Configuration::initialize(this->_currentLoaded);
            }
        }

        using LoadingGui = BaseGUI<cfg::loading::nbWidgets>;

    public:
        void enter() noexcept final
        {
            showEnterScene();
            __configure();
        }

        void resume() noexcept final
        {
            showResumeScene();
        }

        void pause() noexcept final
        {
            showPauseScene();
        }

        void draw() noexcept final
        {
        }

        void leave() noexcept final
        {
            if (_gui.sheet != nullptr)
                GUIManager::destroySheet(*_gui.sheet);
            showLeavingScene();
        }

        void update([[maybe_unused]] double timeSinceLastFrame) noexcept final
        {
            if (_isLoading) {
                _progress = static_cast<float>(_currentLoaded) / _nbFiles;
                auto str = utils::unpackToString(static_cast<unsigned int>(_progress * 100), "%");
                static_cast<CEGUI::ProgressBar&>(_gui[LW::LoadingProgressBar]).setProgress(_progress);
                _gui[LW::Percentage].setText(str);
                if (_currentLoaded == _nbFiles)
                    _isLoading = !_isLoading;
            } else {
                _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Intro);
                leave();
            }
        }

        bool keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept final
        {
            return false;
        }

        bool keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept final
        {
            return false;
        }

        bool mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept final
        {
            return false;
        }

        bool mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept final
        {
            return false;
        }

        bool mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept final
        {
            return false;
        }

    private:
        LoadingGui _gui;
        std::mutex _mutex;
        std::string _filename;
        bool _isLoading{true};
        unsigned int _nbFiles{0};
        std::atomic_uint _currentLoaded{0};
        float _progress{0.0f};
    };
}

#endif //RTYPE_LOADINGSCENE_HPP
