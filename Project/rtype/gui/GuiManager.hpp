//
// Created by roman sztergbaum on 17/09/2017.
//

#ifndef RTYPE_GUIMANAGER_HPP
#define RTYPE_GUIMANAGER_HPP

#include <string_view>
#include <SFML/Graphics/RenderWindow.hpp>
#include <utils/NonCopyable.hpp>
#include <rtype/gui/BaseGui.hpp>
#include <rtype/core/SafeCEGUI.hpp>
#include <rtype/gutils/manager/EventManager.hpp>

namespace rtype
{
    class GuiManager final : public gutils::Receiver<GuiManager>
    {
    public:
        GuiManager(gutils::EventManager &evtMgr, sf::RenderWindow &win, std::string const &root);

    public:
        //! Public static functions
        static void setText(std::pair<CEGUI::Window *, std::string> &&tp) noexcept;
        static void setSize(std::pair<CEGUI::Window *, CEGUI::USize> &&tp) noexcept;
        static void setProperty(std::tuple<CEGUI::Window *, std::string, std::string> &&tp) noexcept;
        static CEGUI::Window *setSheet(std::string const &sheetName) noexcept;
        static void destroySheet(CEGUI::Window &sheet) noexcept;

    public:
        template <typename CB, typename Arg>
        static void applyCEGUIFunc(CB &&cb, Arg &&arg) noexcept(noexcept(cb(std::forward<Arg>(arg))))
        {
            cb(std::forward<Arg>(arg));
        }

        template <typename CB, typename FirstArg, typename ... OtherArgs>
        static void applyCEGUIFunc(CB &&cb, FirstArg &&arg, OtherArgs &&... args) noexcept(noexcept(cb(std::forward<FirstArg>(arg))))
        {
            cb(std::forward<FirstArg>(arg));
            applyCEGUIFunc(std::forward<CB>(cb), std::forward<OtherArgs>(args)...);
        }

        template <size_t size, typename Window = CEGUI::Window *>
        static bool checkWidgets(const std::array<Window, size> &arr) noexcept
        {
            using std::begin;
            using std::end;
            return std::all_of(begin(arr), end(arr), [](auto &&cur) { return cur != nullptr; });
        }

        template <typename Enum, size_t size>
        static bool setGUI(std::string const &layout, BaseGui<size> &gui, logging::Logger &log) noexcept
        {
            gui.sheet = GuiManager::setSheet(layout);
            bool res = (gui.sheet != nullptr);
            if (!res) {
                log(logging::Error) << "from the layout: " << layout << std::endl;
                return res;
            }
            for (auto &&cur : Enum::values()) {
                gui.widgets[cur] = gui.sheet->getChildRecursive(cur.toString());
            }
            res = GuiManager::checkWidgets<size>(gui.widgets);
            if (!res) {
                log(logging::Error) << "Can't get the widgets of the layouts : " << layout << std::endl;
            }
            return res;
        };

    public:
        //! Callbacks
        void receive(const gutils::evt::GuiEvents &evt) noexcept;

    public:
        const CEGUI::System &getSystem() const noexcept;
        const CEGUI::WindowManager &getWindowManager() const noexcept;
        void update(double deltaTime) noexcept;
        void draw() noexcept;

    private:
        void __initRenderer() noexcept;
        void __initMaps() noexcept;
        void __initResources(const std::string &root);
        void __initSystem() noexcept;
        void __initContext() noexcept;
        bool __processEvent(const sf::Event &event, CEGUI::GUIContext &context) noexcept;

    private:
        using KeyMap = std::map<sf::Keyboard::Key, CEGUI::Key::Scan>;
        using MouseButtonMap = std::map<sf::Mouse::Button, CEGUI::MouseButton>;

        KeyMap _keyMap;
        MouseButtonMap _mouseButtonMap;
        CEGUI::System *_system{nullptr};
        CEGUI::OpenGLRenderer *_rdr{nullptr};
        CEGUI::WindowManager *_windowMgr{nullptr};
        CEGUI::GUIContext *_context{nullptr};
        sf::RenderWindow &_win;
        gutils::EventManager &_evtMgr;
    };
}

#endif //RTYPE_GUIMANAGER_HPP
