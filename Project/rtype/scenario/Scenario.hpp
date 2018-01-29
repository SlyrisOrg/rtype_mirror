//
// Created by milerius on 20/01/18.
//

#ifndef RTYPE_SCENARIO_HPP
#define RTYPE_SCENARIO_HPP

#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <log/Logger.hpp>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/entity/EntityManager.hpp>
#include <rtype/scenario/ScenarioAction.hpp>
#include <rtype/entity/ECS.hpp>
#include <rtype/lua/LuaManager.hpp>
#include <rtype/utils/DemoUtils.hpp>
#include <rtype/scenes/EnumScenes.hpp>


namespace scenario
{
    class ScenarioSystem
    {
    private:
        using ParsingCB = std::function<void(const rapidjson::Document &document)>;
        using ParsingMap = std::unordered_map<std::string, ParsingCB>;

    public:
        explicit ScenarioSystem(rtype::EntityManager &ettMgr,
                                gutils::EventManager &evtMgr,
                                rtype::lua::LuaManager &lua,
                                sfutils::ResourceManager<sf::Texture, rtype::demo::SpriteT> &textures) noexcept
                : _ettMgr(ettMgr), _evtMgr(evtMgr), _lua(lua),_textures(textures)
        {
            _parsingMap = std::unordered_map<std::string, ParsingCB>
                {
                    {"music",       std::bind(&ScenarioSystem::_parseMusic, this, std::placeholders::_1)},
                    {"stopMusic",   std::bind(&ScenarioSystem::_parseStopMusic, this, std::placeholders::_1)},
                    {"spawn",       std::bind(&ScenarioSystem::_parseSpawn, this, std::placeholders::_1)},
                    {"wait",        std::bind(&ScenarioSystem::_parseWait, this, std::placeholders::_1)},
                    {"waitKillAll", std::bind(&ScenarioSystem::_parseWaitKillAll, this, std::placeholders::_1)},
                };
        }

        void _parseMusic(const rapidjson::Document &document)
        {
            std::string_view musicStr = std::string_view(document.GetObject()["music"].GetString());
            _log(logging::Debug) << "add music -> " << musicStr << std::endl;
            rtype::Configuration::Music music = musicStr;
            _action.add<scenario::Music>(music, _evtMgr);
        }

        void _parseStopMusic(const rapidjson::Document &document)
        {
            std::string_view musicStr = std::string_view(document.GetObject()["stopMusic"].GetString());
            _log(logging::Debug) << "stop music -> " << musicStr << std::endl;
            rtype::Configuration::Music music = musicStr;
            _action.add<scenario::StopMusic>(music, _evtMgr);
        }

        void _parseSpawn(const rapidjson::Document &document)
        {
            const auto &spawnOBJ = document.GetObject()["spawn"].GetObject();
            std::string ID = spawnOBJ["id"].GetString();
            std::string texture = spawnOBJ["texture"].GetString();
            std::string script = spawnOBJ["IA"].GetString();
            float scale = spawnOBJ["scale"].GetFloat();
            float startY = spawnOBJ["startY"].GetFloat();
            const auto &statOBJ = spawnOBJ["stat"].GetObject();
            rtype::components::Stat stat(statOBJ["hp"].GetUint(), statOBJ["attack"].GetUint(),
                                         statOBJ["defense"].GetUint(), statOBJ["speed"].GetUint(),
                                         statOBJ["shield"].GetUint());
            const auto &argOBJ = spawnOBJ["argument"].GetObject();
            unsigned int nbArg = argOBJ["nbArg"].GetUint();
            std::vector<float> args;
            for (unsigned int i = 1; i <= nbArg; ++i) {
                args.push_back(argOBJ[("Arg" + std::to_string(i)).c_str()].GetFloat());
            }
            _log(logging::Debug) << "add spawn -> " << ID << std::endl;
            _action.add<scenario::Spawn>(std::move(ID),
                                         std::move(texture),
                                         scale,
                                         startY,
                                         std::move(stat),
                                         std::move(script),
                                         nbArg,
                                         std::move(args),
                                         _textures,
                                         _evtMgr);
        }

        void _parseWait(const rapidjson::Document &document)
        {
            unsigned int duration = document.GetObject()["wait"].GetUint();
            _log(logging::Debug) << "add a waiting -> " << duration << std::endl;
            _action.add<scenario::Wait>(duration, _action);
        }

        void _parseWaitKillAll([[maybe_unused]] const rapidjson::Document &document)
        {
            _log(logging::Debug) << "add a waitkillall" << std::endl;
            _action.add<scenario::WaitKillAll>(_action);
        }

        void _parseConfigInside(const rapidjson::Document &document)
        {
            const auto &actionCFG = document["actions"];
            for (const auto &v : actionCFG.GetArray()) {
                std::string memberName = v.GetObject().begin()->name.GetString();
                _log(logging::Debug) << memberName << std::endl;
                rapidjson::Document around;
                around.CopyFrom(v, around.GetAllocator());
                _parsingMap.at(memberName)(around);
            }
        }

        void _parseScenario(const std::string &path) noexcept
        {
            using namespace std::string_literals;
            try {
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
                _parseConfigInside(doc);
            }
            catch (const std::exception &error) {
                _log(logging::Error) << error.what() << std::endl;
                _evtMgr.emit<gutils::evt::GameShutdown>();
            }
        }

        void configure(const std::string &path) noexcept
        {
            _parseScenario(path);
        }

        void update([[maybe_unused]]double timeSinceLastFrame) noexcept
        {
            if (_action.isWaiting() && _action.isWaitDone()) {
                _action.setWait(false);
            }
            while(!(!_action) && !_action.isWaiting()) {
                _action.execute();
            }

            if (!_action)
                _evtMgr.emit<gutils::evt::ChangeScene>(rtype::Scene::Login);

            _ettMgr.for_each<rtype::rtc::Enemy>([this, timeSinceLastFrame](rtype::Entity &ett){
                auto &table = ett.getComponent<rtype::rtc::Lua>().tableName;
                _lua[table]["update"](ett.getID(), timeSinceLastFrame);
            });

            _ettMgr.for_each<rtype::rtc::Allied>([this, timeSinceLastFrame](rtype::Entity &ett){
                auto &table = ett.getComponent<rtype::rtc::Lua>().tableName;
                _lua[table]["update"](ett.getID(), timeSinceLastFrame);
            });
        }

    private:
        rtype::EntityManager &_ettMgr;
        gutils::EventManager &_evtMgr;
        rtype::lua::LuaManager &_lua;
        sfutils::ResourceManager<sf::Texture, rtype::demo::SpriteT> &_textures;
        Action _action{_ettMgr};
        logging::Logger _log{"scenario", logging::Debug};
        ParsingMap _parsingMap;
    };
}

#endif //RTYPE_SCENARIO_HPP
