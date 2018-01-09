//
// Created by doom on 23/12/17.
//

#ifndef RTYPE_ENTITYMANAGER_HPP
#define RTYPE_ENTITYMANAGER_HPP

#include <tuple>
#include <memory>
#include <unordered_map>
#include <meta/List.hpp>
#include <meta/TupleForEach.hpp>

namespace game
{
    namespace details
    {
        template <typename CompsList, template <typename> typename CompsAlloc>
        class Entity
        {
        private:
            using ComponentsList = CompsList;
            using ComponentsPtr = meta::list::Transform<ComponentsList, std::add_pointer_t>;
            using ComponentsPtrTuple = meta::list::Convert<ComponentsPtr, std::tuple>;

            template <typename T>
            using AllocTraits = std::allocator_traits<CompsAlloc<T>>;

        public:
            using AllocatorsList = meta::list::Transform<ComponentsList, CompsAlloc>;
            using AllocatorsTuple = meta::list::Convert<AllocatorsList, std::tuple>;
            using ID = size_t;

            explicit Entity(ID id, AllocatorsTuple &t) noexcept : _id(id), _allocators(t)
            {
                meta::tuple_for_each(_components, [](auto &p) {
                    p = nullptr;
                });
            }

            ~Entity() noexcept
            {
                meta::tuple_for_each(_components, [](auto &p) {
                    delete p;
                    p = nullptr;
                });
            }

            ID getID() const noexcept
            {
                return _id;
            }

            void mark() noexcept
            {
                _marked = true;
            }

            void unmark() noexcept
            {
                _marked = false;
            }

            bool isMarked() const noexcept
            {
                return _marked;
            }

            template <typename T>
            T &getComponent() noexcept
            {
                using namespace meta::list;
                return *std::get<Position<ComponentsList, T>::value>(_components);
            }

            template <typename ...Args>
            std::tuple<std::add_lvalue_reference_t<Args>...> getComponents() noexcept
            {
                return {getComponent<Args>()...};
            }

            template <typename T>
            bool hasComponent() noexcept
            {
                using namespace meta::list;
                return std::get<Position<ComponentsList, T>::value>(_components) != nullptr;
            }

            template <typename T, typename ...Args>
            bool hasComponents() noexcept
            {
                if constexpr (sizeof...(Args) > 0) {
                    return hasComponent<T>() && hasComponents<Args...>();
                }
                return hasComponent<T>();
            }

            template <typename T, typename ...Args>
            T &addComponent(Args &&...args)
            {
                using namespace meta::list;
                constexpr size_t pos = Position<ComponentsList, T>::value;
                auto &al = std::get<pos>(_allocators);

                std::get<pos>(_components) = AllocTraits<T>::allocate(al, 1);
                AllocTraits<T>::construct(al, std::get<pos>(_components), args...);
                return getComponent<T>();
            }

            template <typename T>
            void removeComponent()
            {
                using namespace meta::list;
                constexpr size_t pos = Position<ComponentsList, T>::value;
                auto &al = std::get<pos>(_allocators);

                AllocTraits<T>::destroy(al, std::get<pos>(_components));
                AllocTraits<T>::deallocate(al, std::get<pos>(_components), 1);
                std::get<pos>(_components) = nullptr;
            }

        private:
            ID _id;
            AllocatorsTuple &_allocators;
            ComponentsPtrTuple _components;
            bool _marked{false};
        };
    }

    template <typename ComponentsList, template <typename> typename ComponentsAlloc = std::allocator>
    class EntityManager
    {
    public:
        using Entity = details::Entity<ComponentsList, ComponentsAlloc>;
        using EntityID = typename Entity::ID;
        using AllocatorsTuple = typename Entity::AllocatorsTuple;

    private:
        EntityID __nextID() noexcept
        {
            static EntityID curID{0};

            return curID++;
        }

    public:
        EntityID createEntity() noexcept
        {
            return createEntity(__nextID());
        }

        EntityID createEntity(EntityID id) noexcept
        {
            auto pair = _entities.emplace(id, Entity(id, _allocators));

            return pair.first->first;
        }

        Entity &operator[](EntityID id) noexcept
        {
            return _entities.at(id);
        }

        template <typename ...Types, typename PredT>
        void for_each(PredT &&pred) noexcept(noexcept(pred(std::declval<Entity &>())))
        {
            for (auto &pair : _entities) {
                auto &entity = pair.second;
                if (entity.template hasComponents<Types...>()) {
                    pred(entity);
                }
            }
        }

        void sweepEntities()
        {
            for (auto it = _entities.begin(); it != _entities.end();) {
                if (it->second.isMarked()) {
                    it = _entities.erase(it);
                } else {
                    ++it;
                }
            }
        }

        size_t nbEntities() const noexcept
        {
            return _entities.size();
        }

    private:
        std::unordered_map<EntityID, Entity> _entities;
        AllocatorsTuple _allocators;
    };
}

#endif //RTYPE_ENTITYMANAGER_HPP
