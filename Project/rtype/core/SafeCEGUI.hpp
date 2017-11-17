//
// Created by szterg_r on 10/10/2017.
//

#ifndef GAME_TEMPLATE_SAFECEGUI_HPP
#define GAME_TEMPLATE_SAFECEGUI_HPP

#include <config/Config.hpp>

#ifdef USING_OSX
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <CEGUI/CEGUI.h>

#ifdef USING_OSX
#pragma clang diagnostic pop
#endif

#endif //GAME_TEMPLATE_SAFECEGUI_HPP
