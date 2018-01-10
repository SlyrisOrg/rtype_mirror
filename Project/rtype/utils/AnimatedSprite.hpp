#ifndef SFUTILS_ANIMATEDSPRITE_HPP
#define SFUTILS_ANIMATEDSPRITE_HPP

#include <string>
#include <functional>
#include <ostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace sfutils
{
    class Animation;

    class AnimatedSprite : public sf::Sprite
    {
    public:
        AnimatedSprite(const AnimatedSprite &) = default;

        AnimatedSprite &operator=(const AnimatedSprite &other) noexcept
        {
            setAnimation(other._animation);
            _loop = other._loop;
            _repeat = other._repeat;
            _status = other._status;
            _delta = other._delta;
            return *this;
        }

        using FuncType = std::function<void()>;
        static FuncType defaultFunc;

        enum Status
        {
            Stopped,
            Paused,
            Playing
        };

    public:

        AnimatedSprite(Animation *animation = nullptr, Status status = Playing,
                       const sf::Time &deltaTime = sf::seconds(0.15f), bool loop = true, int repeat = 0,
                       const sf::IntRect& rect = {1,1,1,1}) noexcept;

    public:
        void setAnimation(Animation *animation) noexcept;
        Animation *getAnimation() const noexcept;

        void setFrameTime(sf::Time deltaTime) noexcept;
        sf::Time getFrameTime() const noexcept;

        void setLoop(bool loop) noexcept;
        bool getLoop() const noexcept;

        void setRepeate(int nb) noexcept;
        int getRepeate() const noexcept;

        void play() noexcept;
        void pause() noexcept;
        void stop() noexcept;
        void update(const sf::Time &deltaTime) noexcept;

        Status getStatus() const noexcept;
        void setFrame(size_t index) noexcept;

        FuncType onFinished;

    private:
        void __setFrame(size_t index, bool resetTime) noexcept;
        Animation *_animation;
        sf::Time _delta;
        sf::Time _elapsed;
        bool _loop;
        int _repeat;
        Status _status;
        size_t _currentFrame;
        sf::IntRect _rect;
    };
}

namespace rtype
{
    struct AnimInfo
    {
        bool loop;
        int repeat;
        sfutils::AnimatedSprite::Status status;
        float speed;
        std::string animation;

        friend std::ostream &operator<<(std::ostream &os, const AnimInfo &info) noexcept
        {
            os << "loop: " << info.loop << " repeat: " << info.repeat << " status: " << info.status << " speed: "
               << info.speed << "animation: " << info.animation;
            return os;
        }
    };

    struct MultiAnim
    {
        MultiAnim() noexcept = default;

        MultiAnim(const std::vector<sfutils::AnimatedSprite> &animArray) noexcept :
            animArray(animArray),
            curAnim{animArray[cur]}
        {
        }

        MultiAnim &operator=(std::vector<sfutils::AnimatedSprite> &animArray) noexcept
        {
            this->animArray = animArray;
            this->cur = 0;
            this->max = static_cast<unsigned int>(animArray.size());
            this->curAnim = animArray[cur];
            this->curAnim.setFrame(0);
            return *this;
        }

        void update(const sf::Time &deltaTime) noexcept
        {
            if (curAnim.getStatus() == sfutils::AnimatedSprite::Stopped) {
                if (cur < max)
                    cur++;
                curAnim = animArray[cur];
            }
            curAnim.update(deltaTime);
        }

        std::vector<sfutils::AnimatedSprite> animArray;
        unsigned int cur{0};
        unsigned int max{static_cast<unsigned int>(animArray.size())};
        sfutils::AnimatedSprite curAnim;
    };
}

#endif
