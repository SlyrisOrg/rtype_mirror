set(UTILS ${CMAKE_SOURCE_DIR}/Project/rtype/utils/)

set(RTYPE_UTILS_HEADERS
        ${UTILS}/AnimatedSprite.hpp
        ${UTILS}/Animation.hpp
        ${UTILS}/ResourceManager.hpp
        ${UTILS}/QuadTree.hpp
        ${UTILS}/Action.hpp
        PARENT_SCOPE)

set(RTYPE_UTILS_SOURCES
        ${UTILS}/AnimatedSprite.cpp
        ${UTILS}/Animation.cpp
        PARENT_SCOPE)

if (WIN32)
    source_group("Header Files\\Utils" FILES ${RTYPE_UTILS_HEADERS})
    source_group("Source Files\\Utils" FILES ${RTYPE_UTILS_SOURCES})
endif (WIN32)