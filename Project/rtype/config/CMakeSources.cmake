set(CONFIG ${CMAKE_SOURCE_DIR}/Project/rtype/config)

set(GAME_TEMPLATE_CONFIG_HEADERS
        ${CONFIG}/Configuration.hpp
        ${CONFIG}/LoginConfig.hpp
        ${CONFIG}/ErrorConfig.hpp
        ${CONFIG}/CreateConfig.hpp
        ${CONFIG}/GameConfig.hpp
        ${CONFIG}/ConfigurationMacros.hpp
        ${CONFIG}/ConfigurationBase.hpp
        PARENT_SCOPE
        )

set(GAME_TEMPLATE_CONFIG_SOURCES
        ${CONFIG}/Configuration.cpp
        ${CONFIG}/LoginConfig.cpp
        ${CONFIG}/ErrorConfig.cpp
        ${CONFIG}/CreateConfig.cpp
        ${CONFIG}/GameConfig.cpp
        PARENT_SCOPE
        )