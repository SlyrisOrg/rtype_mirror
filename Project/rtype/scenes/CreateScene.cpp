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
        _gui[CreateWidgets::BackButton].subscribeEvent(pb::EventClicked,
                                                       evt::Subscriber(&CreateScene::__prepareLeave, this));
        _gui[CreateWidgets::CreateButton].subscribeEvent(pb::EventClicked,
                                                         evt::Subscriber(&CreateScene::__createPlayer, this));
        _gui[CreateWidgets::Bheet].subscribeEvent(tb::EventSelectStateChanged,
                                                  evt::Subscriber(&CreateScene::__firstFaction, this));
        _gui[CreateWidgets::Kooy].subscribeEvent(tb::EventSelectStateChanged,
                                                 evt::Subscriber(&CreateScene::__secondFaction, this));
        _gui[CreateWidgets::Maul].subscribeEvent(tb::EventSelectStateChanged,
                                                 evt::Subscriber(&CreateScene::__thirdFaction, this));
    }

    void CreateScene::__unselectOtherFactions(CreateFactionT factionA, CreateFactionT factionB) noexcept
    {
        reinterpret_cast<CEGUI::ToggleButton *>(&_gui[factionA])->setSelected(false);
        reinterpret_cast<CEGUI::ToggleButton *>(&_gui[factionB])->setSelected(false);

        GUIManager::applyCEGUIFunc(&GUIManager::setProperty,
                                   GuiProperty{&_gui[factionA], "NormalImage",
                                               "RTCreate/" + CreateWidgets::toString(factionA) + "_n"},
                                   GuiProperty{&_gui[factionA], "HoverImage",
                                               "RTCreate/" + CreateWidgets::toString(factionA) + "_h"},
                                   GuiProperty{&_gui[factionB], "NormalImage",
                                               "RTCreate/" + CreateWidgets::toString(factionB) + "_n"},
                                   GuiProperty{&_gui[factionB], "HoverImage",
                                               "RTCreate/" + CreateWidgets::toString(factionB) + "_h"});
    }

    void CreateScene::__changeSelectedFaction(CreateFactionT faction, CreateFactionT factionReset,
                                              CreateFactionT secondFactionReset) noexcept
    {
        bool isSelected = reinterpret_cast<CEGUI::ToggleButton *>(&_gui[faction])->isSelected();
        if (isSelected && _gui[CreateWidgets::FactionName].getText().c_str() != CreateWidgets::toString(faction)) {
            _evtMgr.emit<gutils::evt::PlaySoundEffect>(Configuration::SoundEffect::ClickSmooth);
            __oldSelectedFaction = __selectedFaction;
            __selectedFaction = faction;
            _gui[faction].setProperty("NormalImage", "RTCreate/" + CreateWidgets::toString(faction) + "_a");
            _gui[faction].setProperty("HoverImage", "RTCreate/" + CreateWidgets::toString(faction) + "_a");
            _anims = cfg::create::multiAnimMap[CreateWidgets::toString(faction)];
            _gui[CreateWidgets::FactionName].setText(_gui[faction].getName());
            __setStats(faction);
            GUIManager::applyCEGUIFunc(&GUIManager::replaceText,
                                       GuiReplaceText{&_gui[CreateWidgets::FactionHeadLabel],
                                                      CreateWidgets::toString(__oldSelectedFaction),
                                                      CreateWidgets::toString(faction)},
                                       GuiReplaceText{&_gui[CreateWidgets::FactionHeadSpaceshipLabel],
                                                      CreateWidgets::toString(__oldSelectedFaction),
                                                      CreateWidgets::toString(faction)});
            _log(logging::Info) << "Faction selected: " << _gui[CreateWidgets::FactionName].getText() << std::endl;
            __unselectOtherFactions(factionReset, secondFactionReset);
        }
    }

    void CreateScene::__setText() noexcept
    {
        using namespace cfg::create;
        for (auto &&cur : createLabel) {
            const auto &[widgetID, widgetTxt] = cur.second;
            _gui[widgetID].setText(widgetTxt);
        }
    }

    void CreateScene::__setImage() noexcept
    {
        using namespace cfg::create;
        const auto &accepted = utils::unpackToString("RTCreate/", CreateWidgets::toString(CreateWidgets::Bheet), "_a");
        GUIManager::applyCEGUIFunc(&GUIManager::setSize,
                                   std::make_pair(&_gui[CreateWidgets::ChooseFaction], chooseFactionSize));
        GUIManager::applyCEGUIFunc(&GUIManager::setProperty,
                                   GuiProperty{&_gui[CreateWidgets::ChooseFaction], "Image", chooseFactionImage},
                                   GuiProperty{&_gui[CreateWidgets::Bheet], "NormalImage", accepted},
                                   GuiProperty{&_gui[CreateWidgets::Bheet], "HoverImage", accepted});
    }

    bool CreateScene::__setGUI() noexcept
    {
        using namespace cfg::create;
        return GUIManager::setGUI<CreateWidgets, 14>(createLayout, _gui, _log);
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
            _anims = cfg::create::multiAnimMap[CreateWidgets::toString(CreateWidgets::Bheet)];
            _gui[CreateWidgets::CreateButton].setWantsMultiClickEvents(false);
            _gui[CreateWidgets::CreateButton].setMouseAutoRepeatEnabled(false);
            _evtMgr.emit<gutils::evt::PlayMusic>(Configuration::Music::SpaceshipBridgeInstrumentalAmb, true);
            auto ptr = reinterpret_cast<CEGUI::ToggleButton *>(&_gui[CreateWidgets::Bheet]);
            ptr->setSelected(true);
            __setImage();
            __setText();
            __setStats(CreateWidgets::Bheet);
            __firstFaction();
            _bg.setTexture(Configuration::textures.get(Configuration::Sprite::ShipInterior));
        } else {
            __leaveGame();
        }
    }

    //! CEGUI Callbacks
    void CreateScene::__firstFaction() noexcept
    {
        __changeSelectedFaction(CreateWidgets::Bheet, CreateWidgets::Kooy, CreateWidgets::Maul);
    }

    void CreateScene::__secondFaction() noexcept
    {
        __changeSelectedFaction(CreateWidgets::Kooy, CreateWidgets::Bheet, CreateWidgets::Maul);
    }

    void CreateScene::__thirdFaction() noexcept
    {
        __changeSelectedFaction(CreateWidgets::Maul, CreateWidgets::Kooy, CreateWidgets::Bheet);
    }

    void CreateScene::__createPlayer() noexcept
    {
        _gui[CreateWidgets::CreateButton].disable();
        std::error_code ec;
        auto res = API::requestCreatePlayer(_gui[CreateWidgets::Editbox].getText().c_str(), __selectedFaction,
                                            cfg::game::tokenSession, ec).wait();
        if (res == pplx::task_group_status::completed) {
            _gui[CreateWidgets::CreateButton].enable();
            if (!ec) {
                _evtMgr.emit<gutils::evt::ChangeScene>(Scene::Profil);
            }
        }
    }

    void CreateScene::__prepareLeave() noexcept
    {
        __selectedFaction = CreateWidgets::Bheet;
        __oldSelectedFaction = CreateWidgets::Bheet;
        _anims = cfg::create::multiAnimMap[CreateWidgets::toString(CreateWidgets::Bheet)];
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

        std::apply(stats, _stats[faction]);
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
        for (const auto &[widgetID, widgetProgress] : _progress) {
            auto &progress = static_cast<CEGUI::ProgressBar &>(this->_gui[widgetID]);
            if (progress.getProgress() <= widgetProgress) {
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