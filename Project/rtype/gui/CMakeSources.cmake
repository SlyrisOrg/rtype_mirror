set(GUI ${CMAKE_SOURCE_DIR}/Project/rtype/gui/)

set(RTYPE_GUI_HEADERS
        ${GUI}/BaseGUI.hpp
        ${GUI}/GUIManager.hpp
        PARENT_SCOPE)

set(RTYPE_GUI_SOURCES
        ${GUI}/GUIManager.cpp
        PARENT_SCOPE
        )