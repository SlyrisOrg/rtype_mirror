set(SYSTEMS ${CMAKE_SOURCE_DIR}/Project/rtype/systems/)

set(RTYPE_SYSTEMS_HEADERS
        ${SYSTEMS}/SCollision.hpp
        ${SYSTEMS}/SEvents.hpp
        ${SYSTEMS}/SGui.hpp
        ${SYSTEMS}/SScenes.hpp
        ${SYSTEMS}/SystemsEnum.hpp
        ${SYSTEMS}/Systems.hpp
        ${SYSTEMS}/SVideo.hpp
        ${SYSTEMS}/SAudio.hpp
        ${SYSTEMS}/FieldSystem.hpp
        PARENT_SCOPE)

if (WIN32)
    source_group("Header Files\\Systems" FILES ${RTYPE_SYSTEMS_HEADERS})
endif(WIN32)