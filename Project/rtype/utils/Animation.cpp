#include <rtype/utils/Animation.hpp>

namespace sfutils
{
    Animation::Animation(sf::Texture *texture) noexcept : _texture(texture)
    {
    }

    Animation::~Animation() noexcept
    {
    }

    void Animation::setTexture(sf::Texture *texture) noexcept
    {
        _texture = texture;
    }

    sf::Texture *Animation::getTexture() const noexcept
    {
        return _texture;
    }

    Animation &Animation::addFrame(const sf::IntRect &rect) noexcept
    {
        _frames.emplace_back(rect);
        return *this;
    }

    Animation &Animation::addFramesLine(unsigned int numberX,
                                        unsigned int numberY,
                                        unsigned int line,
                                        unsigned int columns) noexcept
    {
        const sf::Vector2u size = _texture->getSize();
        const float delta_x = size.x / float(columns);
        const float delta_y = size.y / float(numberY);

        for (unsigned int i = 0; i < numberX; ++i)
            addFrame(sf::IntRect(static_cast<int>(i * delta_x),
                                 static_cast<int>(line * delta_y),
                                 static_cast<int>(delta_x),
                                 static_cast<int>(delta_y)));
        return *this;
    }

    Animation &Animation::addFramesColumn(int numberX, int numberY, int column) noexcept
    {
        const sf::Vector2u size = _texture->getSize();
        const float delta_x = size.x / float(numberX);
        const float delta_y = size.y / float(numberY);

        for (int i = 0; i < numberY; ++i)
            addFrame(sf::IntRect(static_cast<int>(column * delta_x),
                                 static_cast<int>(i * delta_y),
                                 static_cast<int>(delta_x),
                                 static_cast<int>(delta_y)));
        return *this;
    }

    size_t Animation::size() const noexcept
    {
        return _frames.size();
    }

    const sf::IntRect &Animation::getRect(size_t index) const noexcept
    {
        return _frames[index];
    }
}
