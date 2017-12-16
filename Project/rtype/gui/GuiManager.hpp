//
// Created by roman sztergbaum on 17/09/2017.
//

#ifndef RTYPE_GUIMANAGER_HPP
#define RTYPE_GUIMANAGER_HPP

#include <string_view>
#include <SFML/Graphics/RenderWindow.hpp>
#include <utils/NonCopyable.hpp>
#include <rtype/gui/BaseGUI.hpp>
#include <rtype/core/SafeCEGUI.hpp>
#include <rtype/gutils/manager/EventManager.hpp>

namespace rtype
{
    /**
     * @class GUIManager
     * 
     * @brief contains set of helpers function for cegui manipulation
     */
    class GUIManager final : public gutils::Receiver<GUIManager>
    {
    public:
        GUIManager(gutils::EventManager &evtMgr, sf::RenderWindow &win, std::string const &root);

    public:
        /**
         * @group Different setters for cegui manipulation.
         */
        static void setText(std::pair<CEGUI::Window *, std::string> &&tp) noexcept;
        static void setSize(std::pair<CEGUI::Window *, CEGUI::USize> &&tp) noexcept;
        static CEGUI::Window *setSheet(const std::string &sheetName) noexcept;
        static void setProperty(GuiProperty &&gui) noexcept;

        /**
         * @brief allow you to replace a text in a widget
         * 
         * @param gui contains the CEGUI::Widget, haystack and needle.
         */
        static void replaceText(GuiReplaceText &&gui) noexcept;
        
        /**
         * @brief destroy a CEGUI::Widget
         */
        static void destroySheet(CEGUI::Window &sheet) noexcept;

    public:
        /**
         * @brief apply a cegui callback with his argument.
         * 
         * @tparam Callback cegui helper function
         * @tparam Arg argument of cegui helper function
         * 
         * @overload last recursion of applyCEGUIFunc.
         */
        template <typename Callback, typename Arg>
        static void applyCEGUIFunc(Callback &&cb, Arg &&arg) noexcept(noexcept(cb(std::forward<Arg>(arg))))
        {
            cb(std::forward<Arg>(arg));
        }

        /**
         * @see overload applyCEGUIFunc.
         */
        template <typename Callback, typename FirstArg, typename ...OtherArgs>
        static void applyCEGUIFunc(Callback &&cb, FirstArg &&arg, OtherArgs &&...args) noexcept(noexcept(cb(std::forward<FirstArg>(arg))))
        {
            cb(std::forward<FirstArg>(arg));
            applyCEGUIFunc(std::forward<Callback>(cb), std::forward<OtherArgs>(args)...);
        }

        /**
         * @brief check if a widgets container contains null pointer.
         * 
         * @tparam Size size of the given array.
         * @tparam Window CEGUI::Window
         * 
         * @param arr an array of cegui::window
         * 
         * @return true if all the Window are different than a null pointer, false otherwise.
         */
        template <size_t Size, typename Window = CEGUI::Window *>
        static bool checkWidgets(const std::array<Window, Size> &arr) noexcept
        {
            using std::begin;
            using std::end;
            return std::all_of(begin(arr), end(arr), [](auto &&cur) { return cur != nullptr; });
        }

        /**
         * @brief allow you to set a gui layout as a root CEGUI::Window
         * 
         * @tparam Enum represents the enum type of gui widgets.
         * @tparam Size size of the BaseGUI array.
         * 
         * @param layout name of the layout to load.
         * @param gui an array of CEGUI::Window.
         * @param log a reference from the logger scene (Profil, Create, Login, which scene are loading the current GUI.)
         * 
         * @return true if the gui is set, false otherwise
         */
        template <typename Enum, size_t Size>
        static bool setGUI(std::string const &layout, BaseGUI<Size> &gui, logging::Logger &log) noexcept
        {
            gui.sheet = setSheet(layout);
            bool res = (gui.sheet != nullptr);
            if (!res) {
                log(logging::Error) << "from the layout: " << layout << std::endl;
                return res;
            }
            for (auto &&cur : Enum::values()) {
                gui.widgets[cur] = gui.sheet->getChildRecursive(cur.toString());
            }
            res = GUIManager::checkWidgets<Size>(gui.widgets);
            if (!res) {
                log(logging::Error) << "Can't get the widgets of the layouts : " << layout << std::endl;
            }
            return res;
        };

    public:
        
        /**
         * @brief process the event for the gui (keyboard, mouse, etc).
         */
        void receive(const gutils::evt::GuiEvents &evt) noexcept;

    public:
        /**
         * @group CEGUI getters.
         */
        const CEGUI::System &getSystem() const noexcept;
        const CEGUI::WindowManager &getWindowManager() const noexcept;

        /**
         * @brief update the gui context and the gui system.
         */
        void update(double deltaTime) noexcept;

        /**
         * @brief draw the gui context using OpenGL.
         */
        void draw() noexcept;

    private:

        /*
         * @group internal initializer functions.
         */
        void __initRenderer() noexcept;
        void __initMaps() noexcept;
        void __initResources(const std::string &root);
        void __initSystem() noexcept;
        void __initContext() noexcept;

        /**
         * @internal process the gui event from a given context.
         */
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
