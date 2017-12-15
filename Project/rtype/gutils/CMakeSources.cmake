set(GUTILS ${CMAKE_SOURCE_DIR}/Project/rtype/gutils/)

set(RTYPE_GUTILS_HEADERS
        ${GUTILS}/manager/EventManager.hpp
        ${GUTILS}/manager/SystemManager.hpp
        ${GUTILS}/base/AScene.hpp
        ${GUTILS}/base/BaseSystem.hpp
        ${GUTILS}/event/InsideEvents.hpp
        PARENT_SCOPE)