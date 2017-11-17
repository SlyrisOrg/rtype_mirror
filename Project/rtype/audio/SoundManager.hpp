//
// Created by roman sztergbaum on 25/10/2017.
//

#ifndef RTYPE_MUSICMANAGER_HPP
#define RTYPE_MUSICMANAGER_HPP

#include <deque>
#include <algorithm>
#include <log/Logger.hpp>
#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/gutils/manager/EventManager.hpp>

namespace rtype
{
    namespace lg = logging;
    namespace evt = gutils::evt;

    /**
     * @class SoundManager
     * @brief
     * The SoundManager class allows you to manage all the audio concerns in the rtype,
     * it works with the event manager of game utils.
     * Through its callbacks you can manipulate music, sound effects, make them loop, control their volumes.
     */
    class SoundManager final : public gutils::Receiver<SoundManager>
    {
    public:
        /**
         * @brief The SoundManager constructor explicitly takes an EventManager to be able to subscribe
         * to the events that interest him such as being able to play or stop a music.
         * @param evtMgr a reference to the event Manager.
         */
        explicit SoundManager(gutils::EventManager &evtMgr) noexcept;

    public:
        //! Public member functions

        /**
         * @note
         * The update function will erase the sounds of the queue that stopped playing.
         */
        void update() noexcept;

    public:
        //! Public Event Callbacks

        /**
         * @brief
         * Callback PlaySoundEffect simply plays a SFML sound effect with a certain volume, and can be played in a loop.
         * @param evt PlaySoundEffect
         */
        void receive(const evt::PlaySoundEffect &evt) noexcept;

        /**
         * @brief
         * Callback StopMusic simply stop a SFML Music.
         * @param evt StopMusic
         */
        void receive(const evt::StopMusic &evt) noexcept;

        /**
         * @brief
         * Callback PlaySoundEffect simply plays a SFML Music with a certain volume, and can be played in a loop.
         * @param evt PlaySoundEffect
         */
        void receive(const evt::PlayMusic &evt) noexcept;

    private:
        using SoundQueue = std::deque<sf::Sound>;
    private:
        //! Private members.
        SoundQueue _sndQueue{};
        gutils::EventManager &_evtMgr;
        lg::Logger _log{"AudioManager", lg::Info};
    };
}

#endif //RTYPE_MUSICMANAGER_HPP