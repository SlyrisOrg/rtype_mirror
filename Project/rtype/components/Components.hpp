//
// Created by milerius on 08/01/18.
//

#ifndef RTYPE_COMPONENTS_HPP
#define RTYPE_COMPONENTS_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <rtype/config/Configuration.hpp>
#include <rtype/utils/AnimatedSprite.hpp>
#include <rtype/utils/DemoUtils.hpp>

namespace rtype::demo::field
{
    class SubField;
}

namespace rtype::components
{
    struct Sprite
    {
        template <typename ...Args>
        Sprite(Args &&...args) : sprite(std::forward<Args>(args)...)
        {
        }

        sf::Sprite sprite;
    };

    struct BoundingBox
    {
        // Construct BoundinxBox with Sprite, Rectangle, Circle
        template <typename SFMLObject>
        BoundingBox(const SFMLObject obj) noexcept : AABB(obj.getGlobalBounds()),
                                                     shapeDebug(sf::Vector2f(AABB.width, AABB.height))
        {
            shapeDebug.setFillColor(sf::Color(0, 0, 0, 0));
            shapeDebug.setOutlineThickness(1.0f);
            shapeDebug.setOutlineColor(sf::Color::Red);
            shapeDebug.setPosition(AABB.left, AABB.top);
        }

        //! Cover x,y,width,height / sf::vector2f Position/Size
        BoundingBox(const sf::Vector2f &position, const sf::Vector2f &size,
                    const sf::IntRect &relative = sf::IntRect{1, 1, 1, 1}) noexcept :
            AABB(position, size),
            shapeDebug(sf::Vector2f(AABB.width, AABB.height)),
            relativeAABB(relative)
        {
            shapeDebug.setFillColor(sf::Color(0, 0, 0, 0));
            shapeDebug.setOutlineThickness(1.0f);
            shapeDebug.setOutlineColor(sf::Color::Red);
            shapeDebug.setPosition(AABB.left, AABB.top);
        }

        BoundingBox(float x, float y, float width, float height,
                    const sf::IntRect &relative = {1, 1, 1, 1}) noexcept :
            AABB(x, y, width, height),
            shapeDebug(sf::Vector2f(AABB.width, AABB.height)),
            relativeAABB(relative)
        {
            shapeDebug.setFillColor(sf::Color(0, 0, 0, 0));
            shapeDebug.setOutlineThickness(1.0f);
            shapeDebug.setOutlineColor(sf::Color::Red);
            shapeDebug.setPosition(AABB.left, AABB.top);
        }

        const sf::Vector2f &getPosition() const noexcept
        {
            return shapeDebug.getPosition();
        }

        void setPosition(const sf::Vector2f &position) noexcept
        {
            AABB.left = position.x;
            AABB.top = position.y;
            shapeDebug.setPosition(AABB.left, AABB.top);
        }

        void setPosition(float x, float y) noexcept
        {
            setPosition(sf::Vector2f(x, y));
        }

        sf::FloatRect AABB;
        sf::RectangleShape shapeDebug;
        const sf::FloatRect relativeAABB;
    };

    struct Animation
    {
        template <typename ...Args>
        Animation(Args &&...args) noexcept : anim(std::forward<Args>(args)...)
        {
        }

        sfutils::AnimatedSprite anim;
        int currentAnim;
    };

    //TODO: Deal with template here, should be better
    struct Player
    {
        using Next = int;
        using Prev = int;
        using Cur = int;
        using TransitionMap = std::unordered_map<Cur, std::pair<Prev, Next>>;

        explicit Player(TransitionMap _map) noexcept : map(std::move(_map))
        {
        }

        TransitionMap map;
    };

    struct Bullet
    {
        Bullet() noexcept = default;
    };

    struct SoundEffect
    {
        SoundEffect(Configuration::SoundEffect eff) noexcept : se(eff), buff(Configuration::effects.get(eff))
        {
        }

        Configuration::SoundEffect se;
        sf::SoundBuffer buff;
    };

    enum Direction
    {
        None,
        North,
        South,
        West,
        East,
    };

    struct Movement
    {
        Direction dir{Direction::None};
    };

    struct Speed
    {
        explicit Speed(float _speed) noexcept : speed(_speed)
        {            
        }

        float speed;
    };

    static constexpr const size_t nbLayer = 7;

    enum LayerType
    {
        StarField,
        Fog1,
        Planet1,
        Fog2,
        Planet2,
        GameField,
        Fog3,
    };

    template<LayerType layer>
    struct Layer
    {

    };

    struct FieldItem
    {
        explicit FieldItem(demo::field::SubField *sub, unsigned int itemPckIdx) noexcept
                : subField(sub), itemPackIdx(itemPckIdx)
        {

        }

        demo::field::SubField *subField;
        unsigned int itemPackIdx;
    };

    using StarFieldLayer = Layer<StarField>;
    using Fog1Layer = Layer<Fog1>;
    using Planet1Layer = Layer<Planet1>;
    using Fog2Layer = Layer<Fog2>;
    using Planet2Layer = Layer<Planet2>;
    using GameFieldLayer = Layer<GameField>;
    using Fog3Layer = Layer<Fog3>;
}

#endif //RTYPE_COMPONENTS_HPP
