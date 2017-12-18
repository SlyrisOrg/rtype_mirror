//
// Created by milerius on 01/11/17.
//

#include <rtype/gutils/event/InsideEvents.hpp>
#include <rtype/scenes/CreateScene.hpp>

namespace rtype
{
    //! Private members functions
    void CreateScene::__subscribeEvents() noexcept
    {
        using pb = CEGUI::PushButton;
        using tb = CEGUI::ToggleButton;
        using evt = CEGUI::Event;
        _gui[CW::BackButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&CreateScene::__prepareLeave, this));
        _gui[CW::CreateButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&CreateScene::__createPlayer, this));
        _gui[CW::Bheet].subscribeEvent(tb::EventSelectStateChanged,
                                       evt::Subscriber(&CreateScene::__firstFaction, this));
        _gui[CW::Kooy].subscribeEvent(tb::EventSelectStateChanged,
                                      evt::Subscriber(&CreateScene::__secondFaction, this));
        _gui[CW::Maul].subscribeEvent(tb::EventSelectStateChanged, evt::Subscriber(&CreateScene::__thirdFaction, this));
    }

    void CreateScene::__unselectOtherFactions(CreateFactionT factionA, CreateFactionT factionB) noexcept
    {
        reinterpret_cast<CEGUI::ToggleButton *>(&_gui[factionA])->setSelected(false);
        reinterpret_cast<CEGUI::ToggleButton *>(&_gui[factionB])->setSelected(false);

        GUIManager::applyCEGUIFunc(&GUIManager::setProperty,
                                   GuiProperty{&_gui[factionA], "NormalImage",
                                               "RTCreate/" + CW::toString(factionA) + "_n"},
                                   GuiProperty{&_gui[factionA], "HoverImage",
                                               "RTCreate/" + CW::toString(factionA) + "_h"},
                                   GuiProperty{&_gui[factionB], "NormalImage",
                                               "RTCreate/" + CW::toString(factionB) + "_n"},
                                   GuiProperty{&_gui[factionB], "HoverImage",
                                               "RTCreate/" + CW::toString(factionB) + "_h"});
    }

    void CreateScene::__changeSelectedFaction(CreateFactionT faction, CreateFactionT factionReset,
                                              CreateFactionT secondFactionReset) noexcept
    {
        bool isSelected = reinterpret_cast<CEGUI::ToggleButton *>(&_gui[faction])->isSelected();
        if (isSelected && _gui[CW::FactionName].getText().c_str() != CW::toString(faction)) {
            _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
            __oldSelectedFaction = __selectedFaction;
            __selectedFaction = faction;
            _gui[faction].setProperty("NormalImage", "RTCreate/" + CW::toString(faction) + "_a");
            _gui[faction].setProperty("HoverImage", "RTCreate/" + CW::toString(faction) + "_a");
            _anims = cfg::create::multiAnimMap[CW::toString(faction)];
            _gui[CW::FactionName].setText(_gui[faction].getName());
            __setStats(faction);
            GUIManager::applyCEGUIFunc(&GUIManager::replaceText,
                                       GuiReplaceText{&_gui[CW::FactionHeadLabel], CW::toString(__oldSelectedFaction),
                                                      CW::toString(faction)},
                                       GuiReplaceText{&_gui[CW::FactionHeadSpaceshipLabel],
                                                      CW::toString(__oldSelectedFaction), CW::toString(faction)});
            _log(logging::Info) << "Faction selected: " << _gui[CW::FactionName].getText() << std::endl;
            __unselectOtherFactions(factionReset, secondFactionReset);
        }
    }

    void CreateScene::__setText() noexcept
    {
        using namespace cfg::create;
        for (auto &&cur : createLabel) {
            const auto &[val, str] = cur.second;
            _gui[val].setText(str);
        }
    }

    void CreateScene::__setImage() noexcept
    {
        using namespace cfg::create;
        const auto &accepted = utils::unpackToString("RTCreate/", CW::toString(CW::Bheet), "_a");
        GUIManager::applyCEGUIFunc(&GUIManager::setSize,
                                   std::make_pair(&_gui[CW::ChooseFaction], chooseFactionSize));
        GUIManager::applyCEGUIFunc(&GUIManager::setProperty,
                                   GuiProperty{&_gui[CW::ChooseFaction], "Image", chooseFactionImage},
                                   GuiProperty{&_gui[CW::Bheet], "NormalImage", accepted},
                                   GuiProperty{&_gui[CW::Bheet], "HoverImage", accepted});
    }

    bool CreateScene::__setGUI() noexcept
    {
        using namespace cfg::create;
        return GUIManager::setGUI<CW, 14>(createLayout, _gui, _log);
    }

    void CreateScene::__leaveGame() noexcept
    {
        _evtMgr.emit<gutils::evt::GameShutdown>();
        leave();
    }

    void CreateScene::__configure() noexcept
    {
        auto start = __setGUI();
        if (start) {
            __subscribeEvents();
            _anims = cfg::create::multiAnimMap[CW::toString(CW::Bheet)];
            _gui[CW::CreateButton].setWantsMultiClickEvents(false);
            _gui[CW::CreateButton].setMouseAutoRepeatEnabled(false);
            _evtMgr.emit<gutils::evt::PlayMusic>(Configuration::Music::SpaceshipBridgeInstrumentalAmb, true);
            auto ptr = reinterpret_cast<CEGUI::ToggleButton *>(&_gui[CW::Bheet]);
            ptr->setSelected(true);
            __setImage();
            __setText();
            __setStats(CW::Bheet);
            __firstFaction();
            _bg.setTexture(Configuration::textures.get(Configuration::Sprite::ShipInterior));
        } else {
            __leaveGame();
        }
    }

    //! CEGUI Callbacks
    void CreateScene::__firstFaction() noexcept
    {
        __changeSelectedFaction(CW::Bheet, CW::Kooy, CW::Maul);
    }

    void CreateScene::__secondFaction() noexcept
    {
        __changeSelectedFaction(CW::Kooy, CW::Bheet, CW::Maul);
    }

    void CreateScene::__thirdFaction() noexcept
    {
        __changeSelectedFaction(CW::Maul, CW::Kooy, CW::Bheet);
    }

    void CreateScene::__createPlayer() noexcept
    {
        _gui[CW::CreateButton].disable();
        std::error_code ec;
        auto res = API::requestCreatePlayer(_gui[CW::Editbox].getText().c_str(), __selectedFaction,
                                            cfg::game::tokenSession, ec).wait();
        if (res == pplx::task_group_status::completed) {
            _gui[CW::CreateButton].enable();
            if (!ec) {
                _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
            }
        }
    }

    void CreateScene::__prepareLeave() noexcept
    {
        __selectedFaction = CW::Bheet;
        __oldSelectedFaction = CW::Bheet;
        _anims = cfg::create::multiAnimMap[CW::toString(CW::Bheet)];
        _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Login);
    }

    void CreateScene::__setStats(CreateScene::CreateFactionT faction) noexcept
    {
        auto percentage = [](unsigned int nb, unsigned int max) {
            return static_cast<float>(nb) / max;
        };
        auto stats = [&percentage, this](auto &&pv, auto &&attack, auto &&defense, auto &&support) {
            _progress[pv.widget] = percentage(pv.nb, pv.max);
            _progress[attack.widget] = percentage(attack.nb, attack.max);
            _progress[defense.widget] = percentage(defense.nb, defense.max);
            _progress[support.widget] = percentage(support.nb, support.max);
            for (auto &&cur : _progress) {
                static_cast<CEGUI::ProgressBar &>(this->_gui[cur.first]).setProgress(0);
            }
        };

        std::apply([&stats](auto &&...args) {
            stats(std::forward<decltype(args)>(args)...);
        }, _stats[faction]);
    }

    //! Public member functions
    void CreateScene::enter() noexcept
    {
        showEnterScene();
        __configure();
    }

    void CreateScene::resume() noexcept
    {
        showResumeScene();
    }

    void CreateScene::pause() noexcept
    {
        showPauseScene();
    }

    void CreateScene::leave() noexcept
    {
        if (_gui.sheet != nullptr) {
            GUIManager::destroySheet(*_gui.sheet);
        }
        showLeavingScene();
        _evtMgr.emit<gutils::evt::StopMusic>(Configuration::Music::SpaceshipBridgeInstrumentalAmb);
    }

    void CreateScene::update(double timeSinceLastFrame) noexcept
    {
        sf::Time t1 = sf::seconds(static_cast<float>(timeSinceLastFrame));
        _anims.update(t1);
        for (const auto &[key, value] : _progress) {
            auto &progress = static_cast<CEGUI::ProgressBar &>(this->_gui[key]);
            if (progress.getProgress() <= value) {
                progress.setProgress(static_cast<float>(progress.getProgress() + (0.4f * timeSinceLastFrame)));
            }
        }
    }

    bool CreateScene::keyPressed([[maybe_unused]] const gutils::evt::KeyPressed &evt) noexcept
    {
        return false;
    }

    bool CreateScene::keyReleased([[maybe_unused]] const gutils::evt::KeyReleased &evt) noexcept
    {
        return false;
    }

    bool CreateScene::mouseMoved([[maybe_unused]] const gutils::evt::MouseMoved &evt) noexcept
    {
        return false;
    }

    bool CreateScene::mousePressed([[maybe_unused]] const gutils::evt::MouseButtonPressed &evt) noexcept
    {
        return false;
    }

    bool CreateScene::mouseReleased([[maybe_unused]] const gutils::evt::MouseButtonReleased &evt) noexcept
    {
        return false;
    }

    void CreateScene::draw() noexcept
    {
        _win.draw(_bg);
        _win.draw(_anims.curAnim);
    }
}