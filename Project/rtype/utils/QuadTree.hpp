//
// Created by milerius on 02/01/18.
//

#ifndef RTYPE_QUADTREE_HPP
#define RTYPE_QUADTREE_HPP

#include <array>
#include <unordered_map>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <rtype/entity/ECS.hpp>

namespace rtype
{

    template <typename EMng>
    class QuadTree
    {
        using EntityID = typename EMng::EntityID;

        static constexpr const size_t nbChilds = 4;

        enum Direction
        {
            NorthWest,
            NorthEast,
            SouthWest,
            SouthEast,
        };

    public:

        struct QuadNode
        {
            QuadNode(sf::FloatRect AABB) noexcept : _AABB{AABB},
                                                    _shapeDebug{sf::Vector2f(AABB.width, AABB.height)}
            {
                _shapeDebug.setFillColor(sf::Color::Transparent);
                _shapeDebug.setOutlineThickness(-1.0f);
                _shapeDebug.setOutlineColor(sf::Color::Green);
                _shapeDebug.setPosition(AABB.left, AABB.top);
            }

            std::vector<size_t> _items;
            std::array<std::unique_ptr<QuadNode>, nbChilds> _childs{{nullptr}};
            sf::FloatRect _AABB;
            sf::RectangleShape _shapeDebug;
        };

        QuadTree(sf::FloatRect AABB, EMng &entityMng) noexcept : _root(AABB), _entityMng(entityMng)
        {
        }

    private:
        void __subdivide(QuadNode *node) noexcept
        {
            float x = node->_AABB.left;
            float y = node->_AABB.top;
            float width = node->_AABB.width / 2.0f;
            float height = node->_AABB.height / 2.0f;
            using namespace sf;

            node->_childs[NorthWest] = std::make_unique<QuadNode>(FloatRect(x, y, width, height));
            node->_childs[NorthEast] = std::make_unique<QuadNode>(FloatRect(x + width, y, width, height));
            node->_childs[SouthWest] = std::make_unique<QuadNode>(FloatRect(x, y + height, width, height));
            node->_childs[SouthEast] = std::make_unique<QuadNode>(FloatRect(x + width, y + height, width, height));

            for (auto &&id : node->_items) {
                for (std::unique_ptr<QuadNode> &childPtr : node->_childs) {
                    if (insert(childPtr.get(), id))
                        break;
                }
            }
            node->_items.clear();
        }

    public:
        void clear()
        {
            _entityMng.template for_each<rtc::BoundingBox>([this](rtype::Entity &ett) {
                this->remove(ett.getID());
            });
        }

        bool insert(QuadNode *node, EntityID id) noexcept
        {
            auto &bBox = _entityMng[id].template getComponent<components::BoundingBox>();

            if (!bBox.AABB.intersects(node->_AABB)) {
                return false;
            }

            if (node->_childs[NorthWest] == nullptr && node->_items.size() < nbChilds) {
                node->_items.push_back(id);
                _itemMap[id] = node;
                return true;
            }

            if (node->_childs[NorthWest] == nullptr) {
                __subdivide(node);
            }

            for (std::unique_ptr<QuadNode> &childPtr : node->_childs) {
                if (insert(childPtr.get(), id)) {
                    return true;
                }
            }
            //! Should never happen
            return false;
        }

        inline bool insert(EntityID id) noexcept
        {
            return insert(&_root, id);
        }

#ifdef DEBUG
# define ISNOEXCEPT false
#else
# define ISNOEXCEPT true
#endif

        bool remove(QuadNode *node, EntityID id) noexcept(ISNOEXCEPT)
        {
#undef ISNOEXCEPT
            auto it = std::find(node->_items.begin(), node->_items.end(), id);

            if (it != node->_items.end()) {
                _itemMap.erase(id);
                node->_items.erase(it);
                return true;
            }

            if (_itemMap.find(id) == _itemMap.end())
                return false;

#ifdef DEBUG
            if (node->_childs[NorthWest] == nullptr)
                throw std::runtime_error("Bad tree : object is said to be in tree but no more childs to search for");
#endif

            sf::FloatRect boundingObj = _itemMap.at(id)->_AABB;
            unsigned int subPos = NorthWest;

            if (node->_childs[NorthWest]->_AABB.intersects(boundingObj))
                subPos = NorthWest;
            else if (node->_childs[NorthEast]->_AABB.intersects(boundingObj))
                subPos = NorthEast;
            else if (node->_childs[SouthWest]->_AABB.intersects(boundingObj))
                subPos = SouthWest;
            else if (node->_childs[SouthEast]->_AABB.intersects(boundingObj))
                subPos = SouthEast;

            if (remove(node->_childs[subPos].get(), id)) {
                size_t nbSubItem = 0;
                for (const std::unique_ptr<QuadNode> &childPtr : node->_childs) {
                    if (childPtr->_childs[NorthWest] != nullptr)
                        return false;
                    nbSubItem += childPtr->_items.size();
                }
                if (nbSubItem <= nbChilds) {
                    // add to my items
                    for (std::unique_ptr<QuadNode> &childPtr : node->_childs) {
                        node->_items.insert(node->_items.end(), childPtr->_items.begin(), childPtr->_items.end());
                        childPtr.reset();
                    }

                    for (auto &&id : node->_items) {
                        _itemMap[id] = node;
                    }
                    return true;
                }
            }
            return false;
        }

        inline bool remove(EntityID id) noexcept
        {
            return remove(&_root, id);
        }

        bool move(QuadNode *node, EntityID id) noexcept
        {
            remove(node, id);
            return insert(node, id);
        }

        inline bool move(EntityID id) noexcept
        {
            return move(&_root, id);
        }

        const QuadNode *getNode(EntityID id) const
        {
            return _itemMap.at(id);
        }

        QuadNode *getNode(EntityID id)
        {
            return _itemMap.at(id);
        }

        QuadNode &getRoot() noexcept
        {
            return _root;
        }

        const QuadNode &getRoot() const noexcept
        {
            return _root;
        }

    private:
        QuadNode _root;
        std::unordered_map<EntityID, QuadNode *> _itemMap;
        EMng &_entityMng;
    };
}

#endif //RTYPE_QUADTREE_HPP
