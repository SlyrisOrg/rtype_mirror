//
// Created by milerius on 28/01/18.
//

#ifndef RTYPE_GAMEFIELDSYSTEM_HPP
#define RTYPE_GAMEFIELDSYSTEM_HPP

#include <random>
#include <memory>
#include <log/Logger.hpp>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/scenes/private/ForwardGameSceneUtils.hpp>
#include <rtype/components/Components.hpp>
#include <rtype/entity/ECS.hpp>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <rtype/entity/GameFactory.hpp>

namespace ig
{
    class SubField
    {
    public:
        struct ItemPack
        {
            unsigned int nbItem;
            std::uniform_real_distribution<float> speed{1.0, 1.0};
            std::uniform_real_distribution<float> scale{1.0, 1.0};
            char alpha;
            rtype::components::LayerType layer;
            std::uniform_real_distribution<float> respawn{0.0, 0.0};
        };

        SubField(rtype::EntityManager &ettMgr,
                 sfutils::ResourceManager<sf::Texture, SpriteT> &textures,
                 gutils::EventManager &evtMgr) noexcept :
            _ettMgr(ettMgr),
            _textures(textures),
            _evtMgr(evtMgr)
        {
        }

        std::vector<SpriteT> &sprites(Color color) noexcept
        {
            return _sprites[color];
        }

        void setdistX(float a, float b) noexcept
        {
            _distX.param(std::uniform_real_distribution<float>(a, b).param());
        }

        void setdistY(float a, float b) noexcept
        {
            _distY.param(std::uniform_real_distribution<float>(a, b).param());
        }

        const std::uniform_real_distribution<float> &getDistX() const noexcept
        {
            return _distX;
        }

        const std::uniform_real_distribution<float> &getDistY() const noexcept
        {
            return _distY;
        }

        std::uniform_real_distribution<float> &getDistX() noexcept
        {
            return _distX;
        }

        std::uniform_real_distribution<float> &getDistY() noexcept
        {
            return _distY;
        }

        std::vector<ItemPack> &getItems() noexcept
        {
            return _itemPacks;
        }

        const std::vector<ItemPack> &getItems() const noexcept
        {
            return _itemPacks;
        }

        void setPosSub(unsigned int i) noexcept
        {
            _posSub = i;
        }

        unsigned int getPosSub() const noexcept
        {
            return _posSub;
        }

    private:
        rtype::EntityManager &_ettMgr;
        sfutils::ResourceManager<sf::Texture, SpriteT> &_textures;
        gutils::EventManager &_evtMgr;
        logging::Logger _log{"SubFieldSystem", logging::Debug};
        std::uniform_real_distribution<float> _distX{0.f, 1920.f};
        std::uniform_real_distribution<float> _distY{0.f, 1080.f};
        std::array<std::vector<SpriteT>, Color::size()> _sprites{{std::vector<SpriteT> {},
                                                                     std::vector<SpriteT> {},
                                                                     std::vector<SpriteT> {}}};
        std::vector<ItemPack> _itemPacks{};
        unsigned int _posSub{0};
    };

    class FieldSystem
    {
    public:

        struct Parallax
        {
            std::array<Color, 2> color{{Color::Blue, Color::Red}};
            float globalSpeed{100.0};
            std::array<float, rtype::components::nbLayer> speed{{1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0}};
        };

        FieldSystem(rtype::EntityManager &ettMgr,
                    sfutils::ResourceManager<sf::Texture, SpriteT> &textures,
                    gutils::EventManager &evtMgr) noexcept :
            _ettMgr(ettMgr),
            _textures(textures),
            _evtMgr(evtMgr)
        {
        }

    private:
        void _parseJson(const rapidjson::Document &document) noexcept
        {
            const auto &speedCFG = document["Speed"];
            _parallax.globalSpeed = speedCFG.GetObject()["Global"].GetFloat();
            _parallax.speed[rtype::components::StarField] = speedCFG.GetObject()["StarField"].GetFloat();
            _parallax.speed[rtype::components::Fog1] = speedCFG.GetObject()["Fog1"].GetFloat();
            _parallax.speed[rtype::components::Planet1] = speedCFG.GetObject()["Planet1"].GetFloat();
            _parallax.speed[rtype::components::Fog2] = speedCFG.GetObject()["Fog2"].GetFloat();
            _parallax.speed[rtype::components::Planet2] = speedCFG.GetObject()["Planet2"].GetFloat();
            _parallax.speed[rtype::components::GameField] = speedCFG.GetObject()["GameField"].GetFloat();
            _parallax.speed[rtype::components::Fog3] = speedCFG.GetObject()["Fog3"].GetFloat();

            const auto &fieldCFG = document["Field"];
            for (const auto &subCFG : fieldCFG.GetArray()) {
                auto subField = std::make_unique<SubField>(_ettMgr, _textures, _evtMgr);
                const auto &spriteCFG = subCFG.GetObject()["Sprites"];
                auto getSprites = [&subField](auto &&cfg, Color color) {
                    for (const auto &spriteName : cfg.GetArray()) {
                        std::string_view name = spriteName.GetString();
                        SpriteT sprite = Sprite(name);
                        subField->sprites(color).push_back(std::move(sprite));
                    }
                };
                getSprites(spriteCFG.GetObject()["Green"], Color::Green);
                getSprites(spriteCFG.GetObject()["Blue"], Color::Blue);
                getSprites(spriteCFG.GetObject()["Red"], Color::Red);

                const auto &posSubCFG = subCFG.GetObject()["posSubstract"];
                const auto &posXCFG = subCFG.GetObject()["posXDistribution"];
                const auto &posYCFG = subCFG.GetObject()["posYDistribution"];
                subField->setdistX(posXCFG.GetArray()[0].GetFloat(), posXCFG.GetArray()[1].GetFloat());
                subField->setdistX(posYCFG.GetArray()[0].GetFloat(), posYCFG.GetArray()[1].GetFloat());
                subField->setPosSub(posSubCFG.GetUint());

                const auto &layerCFG = subCFG.GetObject()["Layer"];
                auto getItems = [&subField](auto &&cfg, rtype::components::LayerType layer) {
                    for (const auto &itemPack : cfg.GetArray()) {
                        SubField::ItemPack pack;
                        pack.nbItem = itemPack.GetObject()["nbItem"].GetInt();
                        pack.alpha = itemPack.GetObject()["alpha"].GetInt();
                        const auto &speed = itemPack.GetObject()["speedDistribution"].GetArray();
                        const auto &scale = itemPack.GetObject()["scaleDistribution"].GetArray();
                        const auto &respawn = itemPack.GetObject()["respawnPos"].GetArray();
                        pack.speed.param(
                            std::uniform_real_distribution<float>(speed[0].GetFloat(), speed[1].GetFloat()).param());
                        pack.scale.param(
                            std::uniform_real_distribution<float>(scale[0].GetFloat(), scale[1].GetFloat()).param());
                        pack.respawn.param(std::uniform_real_distribution<float>(respawn[0].GetFloat(),
                                                                                 respawn[1].GetFloat()).param());
                        pack.layer = layer;
                        subField->getItems().push_back(std::move(pack));
                    }
                };
                getItems(layerCFG.GetObject()["StarField"], rtype::components::StarField);
                getItems(layerCFG.GetObject()["Fog1"], rtype::components::Fog1);
                getItems(layerCFG.GetObject()["Planet1"], rtype::components::Planet1);
                getItems(layerCFG.GetObject()["Fog2"], rtype::components::Fog2);
                getItems(layerCFG.GetObject()["Planet2"], rtype::components::Planet2);
                getItems(layerCFG.GetObject()["Fog3"], rtype::components::Fog3);

                _subFields.push_back(std::move(subField));
            }
        }

        void _parseConfig() noexcept
        {
            using namespace std::string_literals;
            try {
                auto path = cfg::configPath + "field/field.json"s;
                rapidjson::Document doc;
                std::ifstream ifs(path);
                rapidjson::IStreamWrapper isw(ifs);
                doc.ParseStream(isw);
                _log(logging::Info) << "Parsing file: " << path << std::endl;
                if (doc.HasParseError()) {
                    _log(logging::Error) << doc.GetParseError() << " " << "Offset : " << doc.GetErrorOffset()
                                         << std::endl;
                    throw std::runtime_error("Parse file -> "s + path + " failed."s);
                }
                _parseJson(doc);
            }
            catch (const std::exception &error) {
                _log(logging::Error) << error.what() << std::endl;
                _evtMgr.emit<gutils::evt::GameShutdown>();
            }
        }

    public:

        void configure() noexcept
        {
            _parseConfig();

            for (auto &sub : _subFields) {
                unsigned int j = 0;
                for (auto[nbItem, speed, scale, alpha, layer, respawn] : sub->getItems()) {
                    (void)respawn;
                    for (unsigned int i = 0; i < nbItem; ++i) {
                        std::vector<SpriteT> &sprite = sub->sprites(_parallax.color[_mt() % 2]);
                        rtype::GameFactory::createFieldItem(_textures.get(sprite[_mt() % sprite.size()]),
                                                     sf::Vector2f(sub->getDistX()(_mt) - sub->getPosSub(),
                                                                  sub->getDistY()(_mt) - sub->getPosSub()),
                                                     _parallax.globalSpeed * speed(_mt) * _parallax.speed[layer],
                                                     scale(_mt), alpha, layer,
                                                     sub.get(), j);
                    }
                    ++j;
                }
            }
        }

        void update(double timeSinceLastFrame) noexcept
        {
            unsigned long rand = _mt() % 10000;
            if (rand <= 1) {
                Color col = Color::Blue;
                if (_parallax.color[0] == col)
                    col = Color::Red;
                if (_parallax.color[1] == col) {
                    if (col == Color::Red)
                        col = Color::Green;
                    else
                        col = Color::Red;
                }
                _parallax.color[rand] = col;
            }
            _ettMgr.for_each<rtype::components::FieldItem<ig::SubField>>([this, &timeSinceLastFrame](rtype::Entity &ett) {
                sf::Sprite &sprite = ett.getComponent<rtype::components::Sprite>().sprite;
                if (sprite.getPosition().x + sprite.getGlobalBounds().width < 0) {
                    ett.mark();
                    SubField *sub = ett.getComponent<rtype::components::FieldItem<ig::SubField>>().subField;
                    unsigned int itemPckIdx = ett.getComponent<rtype::components::FieldItem<ig::SubField>>().itemPackIdx;
                    auto[nbItem, speed, scale, alpha, layer, respawn] = sub->getItems()[itemPckIdx];
                    (void)nbItem;
                    std::vector<SpriteT> &sprite = sub->sprites(_parallax.color[_mt() % 2]);
                    rtype::GameFactory::createFieldItem(_textures.get(sprite[_mt() % sprite.size()]),
                                                 sf::Vector2f(1921.0 + respawn(_mt),
                                                              sub->getDistY()(_mt) - sub->getPosSub()),
                                                 _parallax.globalSpeed * speed(_mt) * _parallax.speed[layer],
                                                 scale(_mt), alpha, layer, sub, itemPckIdx);
                } else {
                    sprite.setPosition(
                        sprite.getPosition().x -
                        (ett.getComponent<rtype::components::Speed>().speed * timeSinceLastFrame),
                        sprite.getPosition().y);
                }
            });
        }

        void clear() noexcept
        {
            _subFields.clear();
        }

    private:
        rtype::EntityManager &_ettMgr;
        sfutils::ResourceManager<sf::Texture, SpriteT> &_textures;
        gutils::EventManager &_evtMgr;
        logging::Logger _log{"FieldSystem", logging::Debug};
        std::vector<std::unique_ptr<SubField>> _subFields;
        std::random_device _rd;
        std::mt19937 _mt{_rd()};
        Parallax _parallax;
    };
}

#endif //RTYPE_GAMEFIELDSYSTEM_HPP
