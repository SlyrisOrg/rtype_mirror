//
// Created by doom on 23/12/17.
//

#ifndef RTYPE_ENTITYMANAGER_HPP
#define RTYPE_ENTITYMANAGER_HPP

#include <tuple>
#include <memory>
#include <unordered_map>
#include <meta/List.hpp>

namespace meta
{
    namespace details
    {
        template <typename Tuple, typename Fctor, size_t ...Indexes>
        void tuple_for_each_helper(Tuple &t, const Fctor &f, std::index_sequence<Indexes...>)
        {
            (f(std::get<Indexes>(t)), ...);
        }
    }

    template <typename ...Types, typename Fctor>
    void tuple_for_each(std::tuple<Types...> &t, const Fctor &f)
    {
        details::tuple_for_each_helper(t, f, std::index_sequence_for<Types...>{});
    }
}

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

            using AllocatorsList = meta::list::Transform<ComponentsList, CompsAlloc>;
            using AllocTraitsList = meta::list::Transform<ComponentsList, std::allocator_traits>;

        public:
            using ID = size_t;

            explicit Entity(ID id) noexcept : _id(id)
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
            Entity &addComponent(Args &&...args)
            {
                using namespace meta::list;
                std::get<Position<ComponentsList, T>::value>(_components) = new T(args...);
                return *this;
            }

            template <typename T>
            void removeComponent()
            {
                using namespace meta::list;
                delete std::get<Position<ComponentsList, T>::value>(_components);
                std::get<Position<ComponentsList, T>::value>(_components) = nullptr;
            }

        private:
            ComponentsPtrTuple _components;
            ID _id;
            bool _marked{false};
        };
    }

    template <typename ComponentsList, template <typename> typename ComponentsAlloc = std::allocator>
    class EntityManager
    {
    public:
        using Entity = details::Entity<ComponentsList, ComponentsAlloc>;
        using EntityID = typename Entity::ID;

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
            auto pair = _entities.emplace(id, Entity(id));

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
    };
}

#endif //RTYPE_ENTITYMANAGER_HPP
