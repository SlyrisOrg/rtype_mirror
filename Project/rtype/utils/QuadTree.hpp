//
// Created by milerius on 02/01/18.
//

#ifndef RTYPE_QUADTREE_HPP
#define RTYPE_QUADTREE_HPP

#include <array>
#include <unordered_map>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace rtype
{

    template <typename Object>
    class QuadTree
    {
        static constexpr const size_t nbChilds = 4;

        enum Direction
        {
            NorthWest,
            NorthEast,
            SouthWest,
            SouthEast,
        };

    public:
        struct Item
        {
            Item(sf::Vector2f size, sf::Vector2f position, const Object *obj) noexcept : objPtr(obj)
            {
                rect.setSize(size);
                rect.setPosition(position);
            }

            sf::RectangleShape rect;
            const Object *objPtr;
        };

        struct QuadNode
        {
            QuadNode(sf::FloatRect AABB) noexcept : _AABB{AABB}
            {
            }

            std::vector<Item> _items;
            std::array<std::unique_ptr<QuadNode>, nbChilds> _childs{{nullptr}};
            sf::FloatRect _AABB;
        };

        QuadTree(sf::FloatRect AABB) noexcept : _root(AABB)
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

            for (auto &&item : node->_items) {
                for (std::unique_ptr<QuadNode> &childPtr : node->_childs) {
                    if (insert(childPtr.get(), item.objPtr))
                        break;
                }
            }
            node->_items.clear();
        }

    public:
        bool insert(QuadNode *node, const Object *objPtr) noexcept
        {
            //remove(objPtr); // make sure an instance can only be represented once in the quadtree
            Item item{objPtr->size, objPtr->position, objPtr};

            if (!item.rect.getGlobalBounds().intersects(node->_AABB)) {
                return false;
            }

            if (node->_childs[NorthWest] == nullptr && node->_items.size() < nbChilds) {
                node->_items.push_back(std::move(item));
                _itemMap[objPtr->id] = node;
                return true;
            }

            if (node->_childs[NorthWest] == nullptr) {
                __subdivide(node);
            }

            for (std::unique_ptr<QuadNode> &childPtr : node->_childs) {
                if (insert(childPtr.get(), objPtr)) {
                    return true;
                }
            }
            //! Should never happen
            return false;
        }

        inline bool insert(const Object *objPtr) noexcept
        {
            return insert(&_root, objPtr);
        }


        bool remove(QuadNode *node, const Object *objPtr) noexcept
        {
            auto it = std::find_if(node->_items.begin(), node->_items.end(), [objPtr](Item &item) {
                return item.objPtr->id == objPtr->id;
            });

            if (it != node->_items.end()) {
                _itemMap.erase(objPtr->id);
                node->_items.erase(it);
                return true;
            }

            if (_itemMap.find(objPtr->id) == _itemMap.end())
                return false;

#ifdef DEBUG
            if (node->_childs[NorthWest] == nullptr)
                throw std::runtime_error("Bad tree : object is said to be in tree but no more childs to search for");
#endif

            sf::FloatRect boundingObj = _itemMap.at(objPtr->id)->_AABB;
            unsigned int subPos = NorthWest;

            if (node->_childs[NorthWest]->_AABB.intersects(boundingObj))
                subPos = NorthWest;
            else if (node->_childs[NorthEast]->_AABB.intersects(boundingObj))
                subPos = NorthEast;
            else if (node->_childs[SouthWest]->_AABB.intersects(boundingObj))
                subPos = SouthWest;
            else if (node->_childs[SouthEast]->_AABB.intersects(boundingObj))
                subPos = SouthEast;

            if (remove(node->_childs[subPos].get(), objPtr)) {
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

                    for (auto &&item : node->_items) {
                        _itemMap[item.objPtr->id] = node;
                    }
                    return true;
                }
            }
            return false;
        }

        inline bool remove(const Object *objPtr) noexcept
        {
            return remove(&_root, objPtr);
        }

        bool move(QuadNode *node, const Object *objPtr) noexcept
        {
            remove(node, objPtr);
            return insert(node, objPtr);
        }

        inline bool move(const Object *objPtr) noexcept
        {
            return move(&_root, objPtr);
        }

        QuadNode *getNode(const size_t id) const
        {
            return _itemMap.at(id);
        }

        QuadNode &getRoot() noexcept
        {
            return  _root;
        }

    private:
        QuadNode _root;
        std::unordered_map<size_t, QuadNode *> _itemMap;
    };
}

#endif //RTYPE_QUADTREE_HPP
