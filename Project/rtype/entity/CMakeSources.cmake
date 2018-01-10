set(ENTITY ${CMAKE_SOURCE_DIR}/Project/rtype/entity/)

set(RTYPE_ENTITY_HEADERS
        ${ENTITY}/EntityManager.hpp
        ${ENTITY}/ECS.hpp
        ${ENTITY}/GameFactory.hpp
        PARENT_SCOPE)

set(RTYPE_ENTITY_SOURCES
        ${ENTITY}/GameFactory.cpp
        PARENT_SCOPE)