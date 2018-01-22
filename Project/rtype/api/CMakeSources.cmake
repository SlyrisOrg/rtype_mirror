set(API ${CMAKE_SOURCE_DIR}/Project/rtype/api/)

set(RTYPE_API_HEADERS
        ${API}/rtype-api.hpp
        ${API}/Player.hpp
        PARENT_SCOPE)

set(RTYPE_API_SOURCES
        ${API}/rtype-api.cpp
        PARENT_SCOPE
        )