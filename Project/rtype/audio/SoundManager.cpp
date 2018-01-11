//
// Created by milerius on 07/11/17.
//

#include <rtype/audio/SoundManager.hpp>

namespace rtype
{
    SoundManager::SoundManager(gutils::EventManager &evtMgr) noexcept : _evtMgr(evtMgr)
    {
        _evtMgr.subscribe<evt::PlayMusic>(*this);
        _evtMgr.subscribe<evt::StopMusic>(*this);
        _evtMgr.subscribe<evt::PlaySoundEffect>(*this);
        _evtMgr.subscribe<evt::ResetMusicVolume>(*this);
    }

    //! Public Member functions.
    void rtype::SoundManager::update() noexcept
    {
        if (!_sndQueue.empty()) {
            _sndQueue.erase(std::remove_if(_sndQueue.begin(), _sndQueue.end(), [](const auto &cur) {
                return cur->getStatus() == sf::Sound::Stopped;
            }), _sndQueue.end());
        }
    }

    //! Callbacks
    void SoundManager::receive(const gutils::evt::PlaySoundEffect &evt) noexcept
    {
        auto &sound = _sndQueue.emplace_back(std::make_unique<sf::Sound>());
        sound->setBuffer(evt.buff);
        sound->setVolume(cfg::game::soundseffectVolume);
        sound->setLoop(evt.loop);
        sound->play();
        _log(lg::Info) << "Playing Sound Effect: " << evt.soundEffectName << ".ogg" << std::endl;
    }

    void SoundManager::receive(const evt::StopMusic &evt) noexcept
    {
        evt.music.stop();
        _log(lg::Info) << "Stopping music: " << evt.musicName << ".ogg" << std::endl;
    }

    void SoundManager::receive([[maybe_unused]] const evt::ResetMusicVolume &evt) noexcept
    {
        evt.music.setVolume(cfg::game::musicVolume);
        _log(lg::Info) << "Reset volume music: " << evt.musicName << ".ogg" << std::endl;
    }

    void SoundManager::receive(const evt::PlayMusic &evt) noexcept
    {
        evt.music.setVolume(cfg::game::musicVolume);
        evt.music.setLoop(evt.loop);
        evt.music.play();
        _log(lg::Info) << "Playing music: " << evt.musicName << ".ogg" << std::endl;
    }
}