set(CORE ${CMAKE_SOURCE_DIR}/Project/rtype/core/)

set(RTYPE_CORE_HEADERS
        ${CORE}/Game.hpp
        ${CORE}/SafeCEGUI.hpp
        ${CORE}/World.hpp
        PARENT_SCOPE)

set(RTYPE_CORE_SOURCES
        ${CORE}/Game.cpp
        PARENT_SCOPE
        )