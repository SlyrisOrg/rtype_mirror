set(CONFIG ${CMAKE_SOURCE_DIR}/Project/rtype/config)

set(RTYPE_CONFIG_HEADERS
        ${CONFIG}/Configuration.hpp
        ${CONFIG}/LoginConfig.hpp
        ${CONFIG}/ErrorConfig.hpp
        ${CONFIG}/CreateConfig.hpp
        ${CONFIG}/GameConfig.hpp
        ${CONFIG}/ConfigurationMacros.hpp
        ${CONFIG}/ConfigurationBase.hpp
        ${CONFIG}/ProfilConfig.hpp
        ${CONFIG}/SpaceshipConfig.hpp
        ${CONFIG}/LoadingConfig.hpp
        PARENT_SCOPE
        )

set(RTYPE_CONFIG_SOURCES
        ${CONFIG}/Configuration.cpp
        ${CONFIG}/LoginConfig.cpp
        ${CONFIG}/ErrorConfig.cpp
        ${CONFIG}/CreateConfig.cpp
        ${CONFIG}/GameConfig.cpp
        ${CONFIG}/ProfilConfig.cpp
        ${CONFIG}/SpaceshipConfig.cpp
        PARENT_SCOPE
        )