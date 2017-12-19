#ifndef SMFL_UTILS_RESOURCEMANAGER_HPP
#define SMFL_UTILS_RESOURCEMANAGER_HPP

#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <string>
#include <memory>
#include <SFML/Audio.hpp>
#include <sfeMovie/Movie.hpp>
#include <rtype/utils/Animation.hpp>

namespace sfutils
{
    template <typename Resource, typename Identifier = int>
    class ResourceManager
    {
    public:
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        ResourceManager() noexcept
        {
            
        };

        template <typename ...Args>
        Resource &load(const Identifier &id, Args &&...args)
        {
            auto ptr = std::make_unique<Resource>();

            if (!ptr->loadFromFile(std::forward<Args>(args)...)) {
                throw std::runtime_error("Impossible to load file");
            }

            if (!_map.emplace(id, std::move(ptr)).second) {
                throw std::runtime_error("Impossible to emplace in map. Object already exists?");
            }

            return *_map[id];
        }

        bool count(const Identifier &id) const noexcept
        {
            return static_cast<bool>(_map.count(id));
        }

        Resource &get(const Identifier &id) const
        {
            return *_map.at(id);
        }

        template <typename ...Args>
        Resource &getOrLoad(const Identifier &id, Args &&...args)
        {
            if (_map.count(id) == 0) {
                return load(id, std::forward<Args>(args)...);
            }
            return get(id);
        }

        void clear() noexcept
        {
            _map.clear();
        }

    private:
        std::unordered_map<Identifier, std::unique_ptr<Resource>> _map;
    };

    template <typename Identifier>
    class ResourceManager<sf::Music, Identifier>
    {
    public:
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        ResourceManager() = default;

        template <typename ...Args>
        sf::Music &load(const Identifier &id, Args &&...args)
        {
            auto ptr = std::make_unique<sf::Music>();

            if (!ptr->openFromFile(std::forward<Args>(args)...)) {
                throw std::runtime_error("Impossible to load file");
            }

            if (!_map.emplace(id, std::move(ptr)).second) {
                throw std::runtime_error("Impossible to emplace in map. Object aleardy exists?");
            }
            return *_map[id];
        }

        bool count(const Identifier &id) const noexcept
        {
            return static_cast<bool>(_map.count(id));
        }

        sf::Music &get(const Identifier &id) const
        {
            return *_map.at(id);
        }

        template <typename ...Args>
        sf::Music &getOrLoad(const Identifier &id, Args &&... args)
        {
            if (_map.count(id) == 0) {
                return load(id, std::forward<Args>(args)...);
            }
            return get(id);
        }

        void clear() noexcept
        {
            _map.clear();
        }

    private:
        std::unordered_map<Identifier, std::unique_ptr<sf::Music>> _map;
    };

    template <typename Identifier>
    class ResourceManager<sfe::Movie, Identifier>
    {
    public:
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        ResourceManager() = default;

        template <typename ...Args>
        sfe::Movie &load(const Identifier &id, Args &&...args)
        {
            std::unique_ptr<sfe::Movie> ptr = std::make_unique<sfe::Movie>();

            if (!ptr->openFromFile(std::forward<Args>(args)...)) {
                throw std::runtime_error("Impossible to load file");
            }

            if (!_map.emplace(id, std::move(ptr)).second) {
                throw std::runtime_error("Impossible to emplace in map. Object aleardy exists?");
            }
            return *_map[id];
        }

        bool count(const Identifier &id) const noexcept
        {
            return static_cast<bool>(_map.count(id));
        }

        sfe::Movie &get(const Identifier &id) const
        {
            return *_map.at(id);
        }

        template <typename ...Args>
        sfe::Movie &getOrLoad(const Identifier &id, Args &&...args)
        {
            if (_map.count(id) == 0) {
                return load(id, std::forward<Args>(args)...);
            }
            return get(id);
        }

        void clear() noexcept
        {
            _map.clear();
        }

    private:
        std::unordered_map<Identifier, std::unique_ptr<sfe::Movie>> _map;
    };

    template <typename Identifier>
    class ResourceManager<Animation, Identifier>
    {
    public:
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        ResourceManager() = default;

        template <typename ...Args>
        Animation &load(const Identifier &id, Args &&...args)
        {
            std::unique_ptr<Animation> ptr = std::make_unique<Animation>(std::forward<Args>(args)...);
            if (!_map.emplace(id, std::move(ptr)).second) {
                throw std::runtime_error("Impossible to emplace in map. Object aleardy exists?");
            }
            return *_map[id];
        }

        bool count(const Identifier &id) const noexcept
        {
            return static_cast<bool>(_map.count(id));
        }

        Animation &get(const Identifier &id) const
        {
            return *_map.at(id);
        }

        template <typename ... Args>
        Animation &getOrLoad(const Identifier &id, Args &&... args)
        {
            if (_map.count(id) == 0) {
                return load(id, std::forward<Args>(args)...);
            }
            return get(id);
        }

        void clear() noexcept
        {
            _map.clear();
        }

    private:
        std::unordered_map<Identifier, std::unique_ptr<Animation>> _map;
    };
}

#endif //SFML_UTILS_RESOURCE_MANAGER_HPP_
