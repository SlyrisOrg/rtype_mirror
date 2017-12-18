//
// Created by milerius on 12/12/17.
//

#ifndef RTYPE_GAMESETTINGS_HPP
#define RTYPE_GAMESETTINGS_HPP

#include <rtype/config/ProfilConfig.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/scenes/private/ForwardEnumProfil.hpp>

namespace rtype
{
    class GameSettings
    {
    public:
        GameSettings(BaseGUI <cfg::profil::nbWidgets> &gui, gutils::EventManager &evtMgr) : _gui(gui), _evtMgr(evtMgr)
        {
        }

    public:

        void init() noexcept
        {
            __initLang();
            __subscribeEvents();
            __misc();
        }

        void show() noexcept
        {
            _gui[ProfilWidgets::OptionsWindow].setVisible(true);
        }

        void hide() noexcept
        {
            _gui[ProfilWidgets::OptionsWindow].setVisible(false);
            __restoreInitialState();

        }

        void resetValue() noexcept
        {
            __resetSoundsEffectLabel();
            __resetMusicLabel();
        }

    private:
        void __resetLabel(CEGUI::Window& label, CEGUI::Window& slider,
                          float& volume, const std::string& labelStr) noexcept
        {
            using namespace cfg::profil;
            volume = std::round(static_cast<CEGUI::Slider &>(slider).getCurrentValue());
            auto str = utils::unpackToString(profileLabel[labelStr].second, volume, "%");
            label.setText(str);
        }

        void __resetSoundsEffectLabel() noexcept
        {
            __resetLabel(_gui[ProfilWidgets::LabelSoundsEffect], _gui[ProfilWidgets::SoundsEffectSlider],
                         _currentSoundEffectVolume, "soundslabel");

        }

        void __resetMusicLabel() noexcept
        {
            __resetLabel(_gui[ProfilWidgets::LabelMusic], _gui[ProfilWidgets::MusicSlider],
                         _currentMusicVolume, "musiclabel");
        }

        void __restoreInitialState() noexcept
        {
            __langTabButtonStateChange();
        }

        void __miscLang() noexcept
        {
            _gui[ProfilWidgets::TabButtonLang].setProperty("NormalImage", "RTOptions/options_tab_hovered");
            _gui[ProfilWidgets::TabButtonLang].setProperty("PushedImage", "RTOptions/options_tab_hovered");
            _gui[ProfilWidgets::TabButtonLang].setName(_gui[ProfilWidgets::TabButtonLang].getName() + "Active");
            _gui[ProfilWidgets::RightPanelLangSettings].setName(_gui[ProfilWidgets::RightPanelLangSettings].getName() + "Active");
        }

        void __miscSound() noexcept
        {
            if (!cfg::game::music)
                static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::MusicCheckBox]).setSelected(true);
            if (!cfg::game::soundseffect)
                static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::SoundsEffectCheckBox]).setSelected(true);
            static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::SoundsEffectSlider]).setCurrentValue(cfg::game::soundseffectVolume);
            static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::MusicSlider]).setCurrentValue(cfg::game::musicVolume);
            _gui[ProfilWidgets::SoundsEffectSlider].getChild("__auto_thumb__")->setAlwaysOnTop(true);
            _gui[ProfilWidgets::MusicSlider].getChild("__auto_thumb__")->setAlwaysOnTop(true);
        }

        void __misc() noexcept
        {
            __miscLang();
            __miscSound();
        }

        void __subscribeEvents() noexcept
        {
            using sr = CEGUI::Slider;
            using pb = CEGUI::PushButton;
            using tg = CEGUI::ToggleButton;
            using evt = CEGUI::Event;
            using gs = GameSettings;

            _gui[ProfilWidgets::fr].subscribeEvent(tg::EventMouseClick, evt::Subscriber(&gs::__frCheck, this));
            _gui[ProfilWidgets::en].subscribeEvent(tg::EventMouseClick, evt::Subscriber(&gs::__enCheck, this));
            _gui[ProfilWidgets::ApplyButton].subscribeEvent(pb::EventClicked, evt::Subscriber(&gs::__applyChange, this));
            _gui[ProfilWidgets::TabButtonLang].subscribeEvent(tg::EventMouseClick,
                                                   evt::Subscriber(&gs::__langTabButtonStateChange, this));
            _gui[ProfilWidgets::TabButtonSounds].subscribeEvent(tg::EventMouseClick,
                                                     evt::Subscriber(&gs::__soundsTabButtonStateChange, this));
            _gui[ProfilWidgets::SoundsEffectSlider].subscribeEvent(sr::EventValueChanged,
                                                        evt::Subscriber(&gs::__soundsEffectChange, this));
            _gui[ProfilWidgets::MusicSlider].subscribeEvent(sr::EventValueChanged,
                                                 evt::Subscriber(&gs::__musicEffectChange, this));
            _gui[ProfilWidgets::MusicCheckBox].subscribeEvent(tg::EventSelectStateChanged,
                                                   evt::Subscriber(&gs::__musicCheckBoxChange, this));
            _gui[ProfilWidgets::SoundsEffectCheckBox].subscribeEvent(tg::EventSelectStateChanged,
                                                          evt::Subscriber(&gs::__soundsEffectCheckBoxChange, this));
        }

        template <typename Functor>
        void __internalCheckRightPlane(Functor &&f, CEGUI::Window *cur, TProfilWidget widget)
        {
            if (f(cur->getName(), CEGUI::String("RightPanel"))) {
                if (cur->getName() == _gui[widget].getName()) {
                    cur->setName(cur->getName() + "Active");
                    cur->setVisible(true);
                } else {
                    auto pos = cur->getName().find("Active");
                    if (pos != CEGUI::String::npos) {
                        CEGUI::String str = cur->getName();
                        str.erase(pos, 6);
                        cur->setName(str);
                        cur->setVisible(false);
                    }
                }
            }
        }

        template <typename Functor>
        void __internalCheckLeftPlane(Functor &&f, CEGUI::Window *cur, TProfilWidget widget)
        {
            if (f(cur->getName(), CEGUI::String("TabButton"))) {
                if (cur->getName() == _gui[widget].getName()) {
                    cur->setName(cur->getName() + "Active");
                    static_cast<CEGUI::ToggleButton *>(cur)->setSelected(true);
                    _gui[widget].setProperty("NormalImage", "RTOptions/options_tab_hovered");
                    _gui[widget].setProperty("PushedImage", "RTOptions/options_tab_hovered");
                } else {
                    auto pos = cur->getName().find("Active");
                    if (pos != CEGUI::String::npos) {
                        CEGUI::String str = cur->getName();
                        str.erase(pos, 6);
                        cur->setName(str);
                        cur->setProperty("NormalImage", "RTOptions/options_tab_normal");
                        cur->setProperty("PushedImage", "RTOptions/options_tab_active");
                        static_cast<CEGUI::ToggleButton *>(cur)->setSelected(false);
                    }
                }
            }
        }

        void __soundsEffectCheckBoxChange() noexcept
        {
            if (static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::SoundsEffectCheckBox]).isSelected()) {
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::SoundsEffectSlider]).setCurrentValue(0.0f);
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::SoundsEffectSlider]).disable();
                _currentSoundEffectVolume = 0.0f;
            } else
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::SoundsEffectSlider]).enable();
        }

        void __musicCheckBoxChange() noexcept
        {
            if (static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::MusicCheckBox]).isSelected()) {
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::MusicSlider]).setCurrentValue(0.0f);
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::MusicSlider]).disable();
                _currentMusicVolume = 0.0f;
            } else
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::MusicSlider]).enable();
        }

        void __soundsEffectChange() noexcept
        {
            __resetSoundsEffectLabel();
            static_cast<CEGUI::ProgressBar &>(_gui[ProfilWidgets::SoundsEffectSliderProgress]).setProgress(
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::SoundsEffectSlider]).getCurrentValue() / 100);
        }

        void __musicEffectChange() noexcept
        {
            __resetMusicLabel();
            static_cast<CEGUI::ProgressBar &>(_gui[ProfilWidgets::MusicSliderProgress]).setProgress(
                static_cast<CEGUI::Slider &>(_gui[ProfilWidgets::MusicSlider]).getCurrentValue() / 100);
        }

        void __checkTabState(TProfilWidget widget, TProfilWidget rightPanel) noexcept
        {
            // Hack for now if this tab button is checked, since when we click on it he change his state ><
            if (_gui[widget].getName() == ProfilWidgets::toString(widget) + "Active") {
                static_cast<CEGUI::ToggleButton &>(_gui[widget]).setSelected(true);
                return;
            }

            const auto startWith = [](const CEGUI::String &s1, const CEGUI::String &s2) {
                return s2.size() <= s1.size() && s1.compare(0, s2.size(), s2) == 0;
            };

            for (auto cur : _gui.widgets) {
                __internalCheckLeftPlane(startWith, cur, widget);
                __internalCheckRightPlane(startWith, cur, rightPanel);
            }
        }

        void __soundsTabButtonStateChange() noexcept
        {
            __checkTabState(ProfilWidgets::TabButtonSounds, ProfilWidgets::RightPanelSoundsSettings);
        }

        void __langTabButtonStateChange() noexcept
        {
            __checkTabState(ProfilWidgets::TabButtonLang, ProfilWidgets::RightPanelLangSettings);
        }

        void __initLang() noexcept
        {
            auto &englishCheck = static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::en]);
            auto &frenchCheck = static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::fr]);
            if (cfg::game::lang == "en") {
                englishCheck.setSelected(true);
                frenchCheck.setSelected(false);
            } else {
                englishCheck.setSelected(false);
                frenchCheck.setSelected(true);
            }
        }

        void __applyLangChange() noexcept
        {
            if (_currentLang != cfg::game::lang) {
                Configuration::changeLang(_currentLang);
                _evtMgr.emit<gutils::evt::ResetText>();
            }
        }

        void __applyMusicChange() noexcept
        {
            if (std::round(_currentMusicVolume) != std::round(cfg::game::musicVolume)) {
                Configuration::changeMusic(_currentMusicVolume);
                _evtMgr.emit<gutils::evt::ResetMusicVolume>(Configuration::Music::MarketInstrumentalAmb);
            }
        }

        void __applySoundsEffectChange() noexcept
        {
            if (std::round(_currentSoundEffectVolume) != std::round(cfg::game::soundseffectVolume)) {
                Configuration::changeSoundsEffect(_currentSoundEffectVolume);
            }
        }

        void __applyChange() noexcept
        {
            __applyLangChange();
            __applyMusicChange();
            __applySoundsEffectChange();
        }

        void __internalLangCheck(CEGUI::ToggleButton &langSelected, CEGUI::ToggleButton &otherLang)
        {
            langSelected.setSelected(true);
            otherLang.setSelected(false);
        }

        void __frCheck() noexcept
        {
            auto &frenchCheck = static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::fr]);
            auto &englishCheck = static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::en]);
            __internalLangCheck(frenchCheck, englishCheck);
            _currentLang = "fr";
        }

        void __enCheck() noexcept
        {
            auto &englishCheck = static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::en]);
            auto &frenchCheck = static_cast<CEGUI::ToggleButton &>(_gui[ProfilWidgets::fr]);
            __internalLangCheck(englishCheck, frenchCheck);
            _currentLang = "en";
        }

    private:
        BaseGUI <cfg::profil::nbWidgets> &_gui;
        std::vector<CEGUI::Window *> _tabButtons;
        std::string _currentLang{cfg::game::lang};
        float _currentSoundEffectVolume{cfg::game::soundseffectVolume};
        float _currentMusicVolume{cfg::game::musicVolume};
        gutils::EventManager &_evtMgr;
    };
}

#endif //RTYPE_GAMESETTINGS_HPP
