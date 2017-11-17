set(GUI ${CMAKE_SOURCE_DIR}/Project/rtype/gui/)

set(RTYPE_GUI_HEADERS
        ${GUI}/BaseGui.hpp
        ${GUI}/GuiManager.hpp
        PARENT_SCOPE)

set(RTYPE_GUI_SOURCES
        ${GUI}/GuiManager.cpp
        PARENT_SCOPE
        )