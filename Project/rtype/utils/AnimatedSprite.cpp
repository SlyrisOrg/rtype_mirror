#include <cassert>
#include <log/Logger.hpp>
#include <rtype/utils/AnimatedSprite.hpp>
#include <rtype/utils/Animation.hpp>

namespace sfutils
{
    AnimatedSprite::FuncType AnimatedSprite::defaultFunc = []() -> void {};

    AnimatedSprite::AnimatedSprite(Animation *animation, Status status, const sf::Time &deltaTime, bool loop,
                                   int repeat, const sf::IntRect &rect) noexcept :
        onFinished(defaultFunc),
        _animation(nullptr),
        _delta(deltaTime),
        _loop(loop),
        _repeat(repeat),
        _status(status),
        _rect(rect)
    {
        setAnimation(animation);
    }

    void AnimatedSprite::setAnimation(Animation *animation, size_t index) noexcept
    {
        if (_animation != animation) {
            _log(logging::Debug) << "Animatio size: -> " << animation->size() << std::endl;
            _animation = animation;
            _elapsed = sf::Time::Zero;
            _currentFrame = 0;
            setFrame(index);
            setTexture(*animation->getTexture());
            if (_rect != sf::IntRect{1, 1, 1, 1})
                setTextureRect(_rect);
        }
    }

    Animation *AnimatedSprite::getAnimation() const noexcept
    {
        return _animation;
    }

    void AnimatedSprite::setFrameTime(sf::Time deltaTime) noexcept
    {
        _delta = deltaTime;
    }

    sf::Time AnimatedSprite::getFrameTime() const noexcept
    {
        return _delta;
    }

    void AnimatedSprite::setLoop(bool loop) noexcept
    {
        _loop = loop;
    }

    bool AnimatedSprite::getLoop() const noexcept
    {
        return _loop;
    }

    void AnimatedSprite::setRepeate(int nb) noexcept
    {
        _repeat = nb;
    }

    int AnimatedSprite::getRepeate() const noexcept
    {
        return _repeat;
    }

    void AnimatedSprite::play() noexcept
    {
        _status = Playing;
    }

    void AnimatedSprite::pause() noexcept
    {
        _status = Paused;
    }

    void AnimatedSprite::stop() noexcept
    {
        _status = Stopped;
        _currentFrame = 0;
        __setFrame(0, true);
    }

    AnimatedSprite::Status AnimatedSprite::getStatus() const noexcept
    {
        return _status;
    }

    void AnimatedSprite::setFrame(size_t index) noexcept
    {
        assert(_animation);
        _currentFrame = index % _animation->size();
        __setFrame(_currentFrame, true);
    }

    void AnimatedSprite::update(const sf::Time &deltaTime) noexcept
    {
        if (_status == Playing && _animation) {
            _elapsed += deltaTime;
            if (_elapsed > _delta) {
                _elapsed -= _delta;
                if (_currentFrame + 1 < _animation->size()) {
                    ++_currentFrame;
                } else {
                    if (_loop)
                        _currentFrame = 0;
                    if (!_loop) {
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

    void AnimatedSprite::__setFrame(size_t index, bool resetTime) noexcept
    {
        if (_animation) {
            sf::IntRect rect = _animation->getRect(index);
            if (_rect != sf::IntRect{1, 1, 1, 1})
                setTextureRect(_rect);
            else
                setTextureRect(rect);
        }

        if (resetTime)
            _elapsed = sf::Time::Zero;
    }

    size_t AnimatedSprite::getCurrentFrame() const noexcept
    {
        return _currentFrame;
    }
}
