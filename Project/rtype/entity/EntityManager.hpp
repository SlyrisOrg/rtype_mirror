//
// Created by doom on 23/12/17.
//

#ifndef RTYPE_ENTITYMANAGER_HPP
#define RTYPE_ENTITYMANAGER_HPP

#include <tuple>
#include <memory>
#include <unordered_map>
#include <meta/List.hpp>

namespace game
{
    namespace details
    {
        template <typename CompsList>
        class Entity
        {
        private:
            using ComponentsList = CompsList;
            using ComponentsPtr = meta::list::Transform<ComponentsList, std::unique_ptr>;
            using ComponentsPtrTuple = meta::list::Convert<ComponentsPtr, std::tuple>;

        public:
            using ID = size_t;

            explicit Entity(ID id) noexcept : _id(id)
            {
            }

            ID getID() const noexcept
            {
                return _id;
            }

            template <typename T>
            T &getComponent() noexcept
            {
                return *std::get<meta::list::Position<ComponentsList, T>::value>(_components).get();
            }

            template <typename T>
            bool hasComponent() noexcept
            {
                return std::get<meta::list::Position<ComponentsList, T>::value>(_components).get() != nullptr;
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
                std::get<meta::list::Position<ComponentsList, T>::value>(_components).reset(new T(args...));
                return *this;
            }

            template <typename T>
            void removeComponent()
            {
                std::get<meta::list::Position<ComponentsList, T>::value>(_components).reset();
            }

        private:
            ComponentsPtrTuple _components;
            ID _id;
        };
    }

    template <typename ComponentsList>
    class EntityManager
    {
    public:
        using Entity = details::Entity<ComponentsList>;
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
        void for_each(PredT &&pred) noexcept(noexcept(pred))
        {
            for (auto &pair : _entities) {
                auto &entity = pair.second;
                if (entity.template hasComponents<Types...>()) {
                    pred(entity);
                }
            }
        }

    private:
        std::unordered_map<EntityID, Entity> _entities;
    };
}

#endif //RTYPE_ENTITYMANAGER_HPP
