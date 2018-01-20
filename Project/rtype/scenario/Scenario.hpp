//
// Created by milerius on 20/01/18.
//

#ifndef RTYPE_SCENARIO_HPP
#define RTYPE_SCENARIO_HPP

#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <log/Logger.hpp>
#include <rtype/gutils/manager/EventManager.hpp>
#include <rtype/scenario/Action.hpp>

namespace scenario
{
    class Scenario
    {
    private:
        using ParsingCB = std::function<void(const rapidjson::Document &document)>;
        using ParsingMap = std::unordered_map<std::string, ParsingCB>;

    public:
        explicit Scenario(const std::string &path, gutils::EventManager &evtMgr) noexcept : _evtMgr(evtMgr)
        {
            _parsingMap = std::unordered_map<std::string, ParsingCB>
                {
                    {"music",       std::bind(&Scenario::_parseMusic, this, std::placeholders::_1)},
                    {"spawn",       std::bind(&Scenario::_parseSpawn, this, std::placeholders::_1)},
                    {"wait",        std::bind(&Scenario::_parseWait, this, std::placeholders::_1)},
                    {"waitkillall", std::bind(&Scenario::_parseWaitKillAll, this, std::placeholders::_1)},
                };
            _parseScenario(path);
        }

        void _parseMusic(const rapidjson::Document &document)
        {
            std::string_view musicStr = std::string_view(document.GetObject()["music"].GetString());
            _log(logging::Debug) << "add music -> " << musicStr << std::endl;
            rtype::Configuration::Music music = musicStr;
            action.add<scenario::Music>(music);
        }

        void _parseSpawn(const rapidjson::Document &document)
        {
            const auto &spawnOBJ = document.GetObject()["spawn"].GetObject();
            std::string ID = spawnOBJ["id"].GetString();
            _log(logging::Debug) << "add spawn -> " << ID << std::endl;
            action.add<scenario::Spawn>(std::move(ID));
        }

        void _parseWait(const rapidjson::Document &document)
        {
            unsigned int duration = document.GetObject()["wait"].GetUint();
            _log(logging::Debug) << "add a waiting -> " << duration << std::endl;
            action.add<scenario::Wait>(duration);
        }

        void _parseWaitKillAll([[maybe_unused]] const rapidjson::Document &document)
        {
            _log(logging::Debug) << "add a waitkillall" << std::endl;
            action.add<scenario::WaitKillAll>();
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

    public:
        Action action;

    private:
        gutils::EventManager &_evtMgr;
        logging::Logger _log{"scenario", logging::Debug};
        ParsingMap _parsingMap;
    };
}

#endif //RTYPE_SCENARIO_HPP
