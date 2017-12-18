#ifndef SFUTILS_ANIMATION_HPP
#define SFUTILS_ANIMATION_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <utils/NonCopyable.hpp>
#include "utils/NonMovable.hpp"

namespace sfutils
{
    class Animation final : private utils::NonCopyable, private utils::NonMovable
    {
    public:
        explicit Animation(sf::Texture *texture = nullptr);
        ~Animation();

    public:
        void setTexture(sf::Texture *texture);
        sf::Texture *getTexture() const;

        Animation &addFrame(const sf::IntRect &rect);
        Animation &addFramesLine(unsigned int numberX, unsigned int numberY, unsigned int line, unsigned int columns);
        Animation &addFramesColumn(int numberX, int numberY, int column);

        size_t size() const noexcept;
        const sf::IntRect &getRect(size_t index) const;

    private:
        friend class AnimatedSprite;

        std::vector<sf::IntRect> _frames;
        sf::Texture *_texture;
    };
}

#endif //SFUTILS_ANIMATION_HPP
