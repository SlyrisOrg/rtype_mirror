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
    }
}

//! Public Member functions.
namespace rtype
{
    void rtype::SoundManager::update() noexcept
    {
        _sndQueue.erase(std::remove_if(_sndQueue.begin(), _sndQueue.end(), [](auto &&cur) {
            return cur.getStatus() == sf::Sound::Stopped;
        }), _sndQueue.end());
    }
}

//! Public Event Callbacks.
namespace rtype
{
    void SoundManager::receive(const gutils::evt::PlaySoundEffect &evt) noexcept
    {
        sf::Sound sound;
        _sndQueue.push_back(sound);
        auto &snd = _sndQueue.back();
        snd.setBuffer(evt.buff);
        snd.setVolume(evt.volume);
        snd.setLoop(evt.loop);
        snd.play();
        _log(lg::Info) << "Playing Sound Effect : " << evt.soundEffectName << ".ogg" << std::endl;
    }

    void SoundManager::receive(const evt::StopMusic &evt) noexcept
    {
        evt.music.stop();
        _log(lg::Info) << "Stopping music : " << evt.musicName << ".ogg" << std::endl;
    }

    void SoundManager::receive(const evt::PlayMusic &evt) noexcept
    {
        evt.music.setVolume(evt.volume);
        evt.music.setLoop(evt.loop);
        evt.music.play();
        _log(lg::Info) << "Playing music : " << evt.musicName << ".ogg" << std::endl;
    }
}