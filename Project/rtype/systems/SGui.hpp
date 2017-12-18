//
// Created by roman sztergbaum on 17/09/2017.
//

#ifndef RTYPE_SGUI_HPP
#define RTYPE_SGUI_HPP

#include <rtype/config/Configuration.hpp>
#include <rtype/gutils/base/BaseSystem.hpp>
#include <rtype/gui/GUIManager.hpp>

namespace rtype
{
    class SGui : public gutils::System<SGui>
    {
    public:
        explicit SGui(gutils::EventManager &mgr, sf::RenderWindow &win, std::string const &root = cfg::guiRoot) noexcept
            : gutils::System<SGui>(mgr, "Gui"), _guiMgr(_evtMgr, win, root)
        {

        }

        void update(double deltaTime) noexcept override
        {
            _guiMgr.update(deltaTime);
        }

        void draw() noexcept override
        {
            _guiMgr.draw();
        }

        ~SGui() noexcept = default;

    private:
        GUIManager _guiMgr;
    };
}

#endif //RTYPE_SGUI_HPP
