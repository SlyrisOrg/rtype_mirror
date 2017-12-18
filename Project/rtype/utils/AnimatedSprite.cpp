#include <cassert>
#include <rtype/utils/AnimatedSprite.hpp>
#include <rtype/utils/Animation.hpp>

namespace sfutils
{
    AnimatedSprite::FuncType AnimatedSprite::defaultFunc = []() -> void {};

    AnimatedSprite::AnimatedSprite(Animation *animation, Status status, const sf::Time &deltaTime, bool loop,
                                   int repeat) :
        onFinished(defaultFunc),
        _animation(nullptr),
        _delta(deltaTime),
        _loop(loop),
        _repeat(repeat),
        _status(status)
    {
        setAnimation(animation);
    }

    void AnimatedSprite::setAnimation(Animation *animation)
    {
        if (_animation != animation) {
            _animation = animation;
            _elapsed = sf::Time::Zero;
            _currentFrame = 0;
            __setFrame(0, true);
            setTexture(*animation->getTexture());
        }
    }

    Animation *AnimatedSprite::getAnimation() const
    {
        return _animation;
    }

    void AnimatedSprite::setFrameTime(sf::Time deltaTime)
    {
        _delta = deltaTime;
    }

    sf::Time AnimatedSprite::getFrameTime() const
    {
        return _delta;
    }

    void AnimatedSprite::setLoop(bool loop)
    {
        _loop = loop;
    }

    bool AnimatedSprite::getLoop() const
    {
        return _loop;
    }

    void AnimatedSprite::setRepeate(int nb)
    {
        _repeat = nb;
    }

    int AnimatedSprite::getRepeate() const
    {
        return _repeat;
    }

    void AnimatedSprite::play()
    {
        _status = Playing;
    }

    void AnimatedSprite::pause()
    {
        _status = Paused;
    }

    void AnimatedSprite::stop()
    {
        _status = Stopped;
        _currentFrame = 0;
        __setFrame(0, true);
    }

    AnimatedSprite::Status AnimatedSprite::getStatus() const
    {
        return _status;
    }

    void AnimatedSprite::setFrame(size_t index)
    {
        assert(_animation);
        _currentFrame = index % _animation->size();
        __setFrame(_currentFrame, true);
    }

    void AnimatedSprite::update(const sf::Time &deltaTime)
    {
        if (_status == Playing && _animation) {
            _elapsed += deltaTime;
            if (_elapsed > _delta) {
                _elapsed -= _delta;
                if (_currentFrame + 1 < _animation->size())
                    ++_currentFrame;
                else {
                    if (_loop)
                        _currentFrame = 0;
                    if (! _loop) {
                        --_repeat;
                        if (_repeat <= 0) {
                            _status = Stopped;
                            onFinished();
                        }
                    }
                }
            }
            __setFrame(_currentFrame, false);
        }
    }

    void AnimatedSprite::__setFrame(size_t index, bool resetTime)
    {
        if (_animation) {
            sf::IntRect rect = _animation->getRect(index);
            setTextureRect(rect);
        }

        if (resetTime)
            _elapsed = sf::Time::Zero;
    }
}
