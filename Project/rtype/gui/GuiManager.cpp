//
// Created by milerius on 17/11/17.
//

#include <rtype/gui/GuiManager.hpp>

//! Constructors
namespace rtype
{
    rtype::GuiManager::GuiManager(gutils::EventManager &evtMgr, sf::RenderWindow &win, const std::string &root) :
        _win(win),
        _evtMgr(evtMgr)
    {
        __initMaps();
        __initRenderer();
        __initSystem();
        __initResources(root);
        __initContext();
        _evtMgr.subscribe<gutils::evt::GuiEvents>(*this);
    }
}

// Public static functions
namespace rtype
{
    void GuiManager::setTextCB(std::pair<CEGUI::Window *, std::string> &&tp) noexcept
    {
        const auto& [widget, value] = tp;
        widget->setText(value);
    }

    void GuiManager::setSizeCB(std::pair<CEGUI::Window *, CEGUI::USize> &&tp) noexcept
    {
        const auto& [widget, value] = tp;
        widget->setSize(value);
    }

    void GuiManager::setPropertyCB(std::tuple<CEGUI::Window *, std::string, std::string> &&tp) noexcept
    {
        const auto& [widget, property, value] = tp;
        widget->setProperty(property, value);
    }

    CEGUI::Window *GuiManager::setSheet(std::string const &sheetName) noexcept
    {
        CEGUI::Window *sheet{nullptr};
        try {
            CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
            sheet = wmgr->loadLayoutFromFile(sheetName);
            CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(sheet);
        }
        catch (const CEGUI::FileIOException &error) {
            std::cerr << error.what() << std::endl;
            sheet = nullptr;
        }
        catch (const CEGUI::InvalidRequestException &error) {
            std::cerr << error.what() << std::endl;
            sheet = nullptr;
        }
        return sheet;
    }

    void GuiManager::destroySheet(CEGUI::Window &sheet) noexcept
    {
        sheet.destroy();
    }
}

namespace rtype
{
    const CEGUI::System &GuiManager::getSystem() const noexcept
    {
        return *_system;
    }

    const CEGUI::WindowManager &GuiManager::getWindowManager() const noexcept
    {
        return *_windowMgr;
    }

    void GuiManager::update(double deltaTime) noexcept
    {
        _system->injectTimePulse(static_cast<float>(deltaTime));
        _context->injectTimePulse(static_cast<float>(deltaTime));
    }

    void GuiManager::draw() noexcept
    {
        _win.pushGLStates();
        _rdr->beginRendering();
        _context->draw();
        _rdr->endRendering();
        _win.popGLStates();
    }
}

//! Private member functions
namespace rtype
{
    void GuiManager::__initRenderer() noexcept
    {
        _rdr = &CEGUI::OpenGLRenderer::bootstrapSystem();
    }

    void GuiManager::__initMaps() noexcept
    {
        _keyMap[sf::Keyboard::Key::A] = CEGUI::Key::A;
        _keyMap[sf::Keyboard::Key::B] = CEGUI::Key::B;
        _keyMap[sf::Keyboard::Key::C] = CEGUI::Key::C;
        _keyMap[sf::Keyboard::Key::D] = CEGUI::Key::D;
        _keyMap[sf::Keyboard::Key::E] = CEGUI::Key::E;
        _keyMap[sf::Keyboard::Key::F] = CEGUI::Key::F;
        _keyMap[sf::Keyboard::Key::G] = CEGUI::Key::G;
        _keyMap[sf::Keyboard::Key::H] = CEGUI::Key::H;
        _keyMap[sf::Keyboard::Key::I] = CEGUI::Key::I;
        _keyMap[sf::Keyboard::Key::J] = CEGUI::Key::J;
        _keyMap[sf::Keyboard::Key::K] = CEGUI::Key::K;
        _keyMap[sf::Keyboard::Key::L] = CEGUI::Key::L;
        _keyMap[sf::Keyboard::Key::M] = CEGUI::Key::M;
        _keyMap[sf::Keyboard::Key::N] = CEGUI::Key::N;
        _keyMap[sf::Keyboard::Key::O] = CEGUI::Key::O;
        _keyMap[sf::Keyboard::Key::P] = CEGUI::Key::P;
        _keyMap[sf::Keyboard::Key::Q] = CEGUI::Key::Q;
        _keyMap[sf::Keyboard::Key::R] = CEGUI::Key::R;
        _keyMap[sf::Keyboard::Key::S] = CEGUI::Key::S;
        _keyMap[sf::Keyboard::Key::T] = CEGUI::Key::T;
        _keyMap[sf::Keyboard::Key::U] = CEGUI::Key::U;
        _keyMap[sf::Keyboard::Key::V] = CEGUI::Key::V;
        _keyMap[sf::Keyboard::Key::W] = CEGUI::Key::W;
        _keyMap[sf::Keyboard::Key::X] = CEGUI::Key::X;
        _keyMap[sf::Keyboard::Key::Y] = CEGUI::Key::Y;
        _keyMap[sf::Keyboard::Key::Z] = CEGUI::Key::Z;
        _keyMap[sf::Keyboard::Key::Num0] = CEGUI::Key::Zero;
        _keyMap[sf::Keyboard::Key::Num1] = CEGUI::Key::One;
        _keyMap[sf::Keyboard::Key::Num2] = CEGUI::Key::Two;
        _keyMap[sf::Keyboard::Key::Num3] = CEGUI::Key::Three;
        _keyMap[sf::Keyboard::Key::Num4] = CEGUI::Key::Four;
        _keyMap[sf::Keyboard::Key::Num5] = CEGUI::Key::Five;
        _keyMap[sf::Keyboard::Key::Num6] = CEGUI::Key::Six;
        _keyMap[sf::Keyboard::Key::Num7] = CEGUI::Key::Seven;
        _keyMap[sf::Keyboard::Key::Num8] = CEGUI::Key::Eight;
        _keyMap[sf::Keyboard::Key::Num9] = CEGUI::Key::Nine;
        _keyMap[sf::Keyboard::Key::Escape] = CEGUI::Key::Escape;
        _keyMap[sf::Keyboard::Key::LControl] = CEGUI::Key::LeftControl;
        _keyMap[sf::Keyboard::Key::LShift] = CEGUI::Key::LeftShift;
        _keyMap[sf::Keyboard::Key::LAlt] = CEGUI::Key::LeftAlt;
        _keyMap[sf::Keyboard::LSystem] = CEGUI::Key::LeftWindows;
        _keyMap[sf::Keyboard::Key::RControl] = CEGUI::Key::RightControl;
        _keyMap[sf::Keyboard::Key::RShift] = CEGUI::Key::RightShift;
        _keyMap[sf::Keyboard::Key::RAlt] = CEGUI::Key::RightAlt;
        _keyMap[sf::Keyboard::RSystem] = CEGUI::Key::RightWindows;
        _keyMap[sf::Keyboard::Key::LBracket] = CEGUI::Key::LeftBracket;
        _keyMap[sf::Keyboard::Key::RBracket] = CEGUI::Key::RightBracket;
        _keyMap[sf::Keyboard::Key::SemiColon] = CEGUI::Key::Semicolon;
        _keyMap[sf::Keyboard::Key::Comma] = CEGUI::Key::Comma;
        _keyMap[sf::Keyboard::Key::Period] = CEGUI::Key::Period;
        _keyMap[sf::Keyboard::Quote] = CEGUI::Key::Apostrophe;
        _keyMap[sf::Keyboard::Key::Slash] = CEGUI::Key::Slash;
        _keyMap[sf::Keyboard::Key::BackSlash] = CEGUI::Key::Backslash;
        _keyMap[sf::Keyboard::Tilde] = CEGUI::Key::Grave;
        _keyMap[sf::Keyboard::Key::Equal] = CEGUI::Key::Equals;
        _keyMap[sf::Keyboard::Key::Dash] = CEGUI::Key::Minus;
        _keyMap[sf::Keyboard::Key::Space] = CEGUI::Key::Space;
        _keyMap[sf::Keyboard::Key::Return] = CEGUI::Key::Return;
        _keyMap[sf::Keyboard::BackSpace] = CEGUI::Key::Backspace;
        _keyMap[sf::Keyboard::Key::Tab] = CEGUI::Key::Tab;
        _keyMap[sf::Keyboard::Key::PageUp] = CEGUI::Key::PageUp;
        _keyMap[sf::Keyboard::Key::PageDown] = CEGUI::Key::PageDown;
        _keyMap[sf::Keyboard::Key::End] = CEGUI::Key::End;
        _keyMap[sf::Keyboard::Key::Home] = CEGUI::Key::Home;
        _keyMap[sf::Keyboard::Key::Insert] = CEGUI::Key::Insert;
        _keyMap[sf::Keyboard::Key::Delete] = CEGUI::Key::Delete;
        _keyMap[sf::Keyboard::Key::Add] = CEGUI::Key::Add;
        _keyMap[sf::Keyboard::Key::Subtract] = CEGUI::Key::Subtract;
        _keyMap[sf::Keyboard::Key::Multiply] = CEGUI::Key::Multiply;
        _keyMap[sf::Keyboard::Key::Divide] = CEGUI::Key::Divide;
        _keyMap[sf::Keyboard::Key::Left] = CEGUI::Key::ArrowLeft;
        _keyMap[sf::Keyboard::Key::Right] = CEGUI::Key::ArrowRight;
        _keyMap[sf::Keyboard::Key::Up] = CEGUI::Key::ArrowUp;
        _keyMap[sf::Keyboard::Key::Down] = CEGUI::Key::ArrowDown;
        _keyMap[sf::Keyboard::Key::Numpad0] = CEGUI::Key::Numpad0;
        _keyMap[sf::Keyboard::Key::Numpad1] = CEGUI::Key::Numpad1;
        _keyMap[sf::Keyboard::Key::Numpad2] = CEGUI::Key::Numpad2;
        _keyMap[sf::Keyboard::Key::Numpad3] = CEGUI::Key::Numpad3;
        _keyMap[sf::Keyboard::Key::Numpad4] = CEGUI::Key::Numpad4;
        _keyMap[sf::Keyboard::Key::Numpad5] = CEGUI::Key::Numpad5;
        _keyMap[sf::Keyboard::Key::Numpad6] = CEGUI::Key::Numpad6;
        _keyMap[sf::Keyboard::Key::Numpad7] = CEGUI::Key::Numpad7;
        _keyMap[sf::Keyboard::Key::Numpad8] = CEGUI::Key::Numpad8;
        _keyMap[sf::Keyboard::Key::Numpad9] = CEGUI::Key::Numpad9;
        _keyMap[sf::Keyboard::Key::F1] = CEGUI::Key::F1;
        _keyMap[sf::Keyboard::Key::F2] = CEGUI::Key::F2;
        _keyMap[sf::Keyboard::Key::F3] = CEGUI::Key::F3;
        _keyMap[sf::Keyboard::Key::F4] = CEGUI::Key::F4;
        _keyMap[sf::Keyboard::Key::F5] = CEGUI::Key::F5;
        _keyMap[sf::Keyboard::Key::F6] = CEGUI::Key::F6;
        _keyMap[sf::Keyboard::Key::F7] = CEGUI::Key::F7;
        _keyMap[sf::Keyboard::Key::F8] = CEGUI::Key::F8;
        _keyMap[sf::Keyboard::Key::F9] = CEGUI::Key::F9;
        _keyMap[sf::Keyboard::Key::F10] = CEGUI::Key::F10;
        _keyMap[sf::Keyboard::Key::F11] = CEGUI::Key::F11;
        _keyMap[sf::Keyboard::Key::F12] = CEGUI::Key::F12;
        _keyMap[sf::Keyboard::Key::F13] = CEGUI::Key::F13;
        _keyMap[sf::Keyboard::Key::F14] = CEGUI::Key::F14;
        _keyMap[sf::Keyboard::Key::F15] = CEGUI::Key::F15;
        _keyMap[sf::Keyboard::Key::Pause] = CEGUI::Key::Pause;
        _mouseButtonMap[sf::Mouse::Button::Left] = CEGUI::LeftButton;
        _mouseButtonMap[sf::Mouse::Button::Right] = CEGUI::RightButton;
        _mouseButtonMap[sf::Mouse::Button::Middle] = CEGUI::MiddleButton;
        _mouseButtonMap[sf::Mouse::Button::XButton1] = CEGUI::X1Button;
        _mouseButtonMap[sf::Mouse::Button::XButton2] = CEGUI::X2Button;
    }

    void GuiManager::__initResources(const std::string &root)
    {
        auto rp = static_cast<CEGUI::DefaultResourceProvider *>(_system->getResourceProvider());
        rp->setResourceGroupDirectory("fonts", root + "fonts/");
        rp->setResourceGroupDirectory("imagesets", root + "imagesets/");
        rp->setResourceGroupDirectory("layouts", root + "layouts/");
        rp->setResourceGroupDirectory("looknfeels", root + "looknfeel/");
        rp->setResourceGroupDirectory("schemes", root + "schemes/");
        rp->setResourceGroupDirectory("lua_scripts", root + "lua_scripts/");
        CEGUI::Font::setDefaultResourceGroup("fonts");
        CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
        CEGUI::WindowManager::setDefaultResourceGroup("layouts");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
        CEGUI::Scheme::setDefaultResourceGroup("schemes");
        CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

        CEGUI::XMLParser *parser = _system->getXMLParser();
        if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
            parser->setProperty("SchemaDefaultResourceGroup", "schemas");

        CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("ODSkin.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("ASSkin.scheme");
        CEGUI::SchemeManager::getSingleton().createFromFile("RType.scheme");
        _system->getDefaultGUIContext().getMouseCursor().setDefaultImage("RTMainElements/Cursors/mouse_n");
    }

    void GuiManager::__initSystem() noexcept
    {
        _system = CEGUI::System::getSingletonPtr();
        _windowMgr = CEGUI::WindowManager::getSingletonPtr();
    }

    void GuiManager::__initContext() noexcept
    {
        _context = &CEGUI::System::getSingletonPtr()->getDefaultGUIContext();
    }

    bool GuiManager::__processEvent(const sf::Event &event, CEGUI::GUIContext &context) noexcept
    {
        bool res = false;
        switch (event.type) {
            case sf::Event::Resized :
                _system->notifyDisplaySizeChanged(
                    CEGUI::Sizef(static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
                break;
            case sf::Event::KeyPressed:
                res = context.injectKeyDown(_keyMap[event.key.code]);
                break;
            case sf::Event::KeyReleased:
                res = context.injectKeyUp(_keyMap[event.key.code]);
                break;
            case sf::Event::TextEntered:
                res = context.injectChar(event.text.unicode);
                break;
            case sf::Event::MouseMoved:
                res = context.injectMousePosition(static_cast<float>(event.mouseMove.x),
                                                  static_cast<float>(event.mouseMove.y));
                break;
            case sf::Event::MouseButtonPressed:
                res = context.injectMouseButtonDown(_mouseButtonMap[event.mouseButton.button]);
                break;
            case sf::Event::MouseButtonReleased:
                res = context.injectMouseButtonUp(_mouseButtonMap[event.mouseButton.button]);
                break;
            case sf::Event::MouseWheelMoved:
                res = context.injectMouseWheelChange(static_cast<float>(event.mouseWheel.delta));
                break;
            default :
                res = false;
                break;
        }
        return res;
    }
}