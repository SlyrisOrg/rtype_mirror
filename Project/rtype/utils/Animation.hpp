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
        explicit Animation(sf::Texture *texture = nullptr) noexcept;
        ~Animation() noexcept;

        void setTexture(sf::Texture *texture) noexcept;
        sf::Texture *getTexture() const noexcept;

        Animation &addFrame(const sf::IntRect &rect) noexcept;
        Animation &addFramesLine(unsigned int numberX, unsigned int numberY,
                                 unsigned int line, unsigned int columns) noexcept;
        Animation &addFramesColumn(int numberX, int numberY, int column) noexcept;

        size_t size() const noexcept;
        const sf::IntRect &getRect(size_t index) const noexcept;

    private:
        friend class AnimatedSprite;
        std::vector<sf::IntRect> _frames;
        sf::Texture *_texture;
    };
}

#endif //SFUTILS_ANIMATION_HPP
