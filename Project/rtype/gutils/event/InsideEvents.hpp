//
// Created by roman sztergbaum on 13/09/2017.
//

#ifndef GUTILS_INSIDE_EVENTS_HPP
#define GUTILS_INSIDE_EVENTS_HPP

#include <string_view>
#include <utility>
#include <SFML/Window/Event.hpp>
#include <log/Logger.hpp>
#include <utils/StringHelpers.hpp>
#include <rtype/config/Configuration.hpp>
#include <rtype/scenes/EnumScenes.hpp>
#include <rtype/protocol/MatchMaking.hpp>

namespace gutils
{
    namespace lg = logging;
    namespace clr = utils;
    using namespace std::string_literals;

    struct InsideEvents
    {
    protected:
        void showEvents(std::string_view str) const noexcept
        {
            _log(lg::Debug) << "Events " << str << " triggered." << std::endl;
        }

        lg::Logger _log{"InsideEvents"s, lg::Debug};
    };
}

namespace gutils::evt
{
    using namespace std::string_view_literals;

    struct FullScreen final : public gutils::InsideEvents
    {
        FullScreen() noexcept
        {
            showEvents("FullScreen");
        }
    };

    struct JoinQueue final : public gutils::InsideEvents
    {
        JoinQueue() noexcept
        {
            showEvents("JoinQueue");
        }
    };

    struct AddPlayerQueue final : public gutils::InsideEvents
    {
        explicit AddPlayerQueue(matchmaking::PlayerInfo &&_info) noexcept : info(std::move(_info))
        {

        }

        matchmaking::PlayerInfo info;
    };

    struct ResetMusicVolume final : public gutils::InsideEvents
    {
        explicit ResetMusicVolume(rtype::Configuration::Music msc) noexcept :
            music(rtype::Configuration::musics.get(msc)),
            musicName(msc.toString())
        {
            showEvents("Reset volume Music"sv);
        }

        sf::Music &music;
        std::string musicName;
    };

    struct ResetText final : public gutils::InsideEvents
    {
        ResetText() noexcept
        {
            showEvents("ResetText"sv);
        }
    };

    struct RestoreDefault final : public gutils::InsideEvents
    {
        explicit RestoreDefault(sf::Window &win) noexcept : win(win)
        {
            showEvents("RestoreDefault"sv);
        }

        sf::Window &win;
    };

    struct StopMusic final : public gutils::InsideEvents
    {
        explicit StopMusic(rtype::Configuration::Music msc) noexcept :
            music(rtype::Configuration::musics.get(msc)),
            musicName(msc.toString())
        {
            showEvents("StopMusic"sv);
        }

        sf::Music &music;
        std::string musicName;
    };

    struct PlayMusic final : public gutils::InsideEvents
    {
        explicit PlayMusic(rtype::Configuration::Music msc, bool loop = false) noexcept :
            music(rtype::Configuration::musics.get(msc)),
            loop(loop),
            musicName(msc.toString())
        {
            showEvents("PlayMusic"sv);
        }

        sf::Music &music;
        bool loop;
        std::string musicName;
    };

    struct PlaySoundEffect final : public gutils::InsideEvents
    {
        explicit PlaySoundEffect(rtype::Configuration::SoundEffect eff, bool loop = false) noexcept :
            buff(rtype::Configuration::effects.get(eff)),
            loop(loop),
            soundEffectName(eff.toString())
        {
            showEvents("PlaySoundEffect"sv);
        }

        PlaySoundEffect(rtype::Configuration::SoundEffect eff, const sf::SoundBuffer &buff,
                        bool loop = false) noexcept : buff(buff), loop(loop), soundEffectName(eff.toString())
        {
            showEvents("PlaySoundEffect"sv);
        }

        const sf::SoundBuffer &buff;
        bool loop;
        std::string soundEffectName;
    };

    struct StopVideo final : public gutils::InsideEvents
    {
        StopVideo() noexcept
        {
            showEvents("StopVideo"sv);
        }
    };

    struct LaunchVideo final : public gutils::InsideEvents
    {
        explicit LaunchVideo(rtype::Configuration::Video vid, rtype::Scene scene) noexcept :
            ptr(std::addressof(rtype::Configuration::videos.get(vid))),
            scene(scene),
            videoName(vid.toString())
        {
            showEvents("LaunchVideo"sv);
        }

        sfe::Movie *ptr;
        rtype::Scene scene;
        std::string videoName;
    };

    struct ChangeScene final : public gutils::InsideEvents
    {
        explicit ChangeScene(rtype::Scene scene) noexcept :
            sceneName(scene)
        {
            showEvents("ChangeScene"sv);
        }

        rtype::Scene sceneName;
    };

    struct RemoveScene final : public gutils::InsideEvents
    {
        explicit RemoveScene(rtype::Scene scene) noexcept :
            sceneName(scene)
        {
            showEvents("ChangeScene"sv);
        }

        rtype::Scene sceneName;
    };

    struct GuiEvents final : public gutils::InsideEvents
    {
        explicit GuiEvents(sf::Event &event) noexcept : evt(event)
        {
        }

        sf::Event &evt;
    };

    struct EnterScene final : public gutils::InsideEvents
    {
        explicit EnterScene(rtype::Scene value) noexcept : scene(value)
        {
            showEvents("EnterScene"sv);
        }

        rtype::Scene scene;
    };

    struct GameShutdown final : public gutils::InsideEvents
    {
        GameShutdown() noexcept
        {
            showEvents("GameShutDown"sv);
        }
    };

    struct WindowResized final : public gutils::InsideEvents
    {
        explicit WindowResized(unsigned int width, unsigned int height) noexcept :
            size{width, height}
        {
            showEvents("WindowResized"sv);
        }

        sf::Vector2u size;
    };

    struct KeyPressed final : public gutils::InsideEvents
    {
        explicit KeyPressed(const sf::Event::KeyEvent &evt) noexcept :
            key(evt.code)
        {
        }

        sf::Keyboard::Key key;
    };

    struct KeyReleased final : public gutils::InsideEvents
    {
        explicit KeyReleased(const sf::Event::KeyEvent &evt) noexcept :
            key(evt.code)
        {
        }

        sf::Keyboard::Key key;
    };

    struct MouseMoved final : public gutils::InsideEvents
    {
        explicit MouseMoved(const sf::Event::MouseMoveEvent &evt) noexcept :
            pos(evt.x, evt.y)
        {
        }

        sf::Vector2i pos;
    };

    struct MouseButtonPressed final : public gutils::InsideEvents
    {
        explicit MouseButtonPressed(const sf::Event::MouseButtonEvent &evt) noexcept :
            mouseButton(evt)
        {
        }

        sf::Event::MouseButtonEvent mouseButton;
    };

    struct MouseButtonReleased final : public gutils::InsideEvents
    {
        explicit MouseButtonReleased(const sf::Event::MouseButtonEvent &evt) noexcept :
            mouseButton(evt)
        {
        }

        sf::Event::MouseButtonEvent mouseButton;
    };
}

#endif //GUTILS_INSIDE_EVENTS_HPP