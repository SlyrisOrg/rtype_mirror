set(AUDIO ${CMAKE_SOURCE_DIR}/Project/rtype/audio/)

set(RTYPE_AUDIO_HEADERS
        ${AUDIO}/SoundManager.hpp
        PARENT_SCOPE)

set(RTYPE_AUDIO_SOURCES
        ${AUDIO}/SoundManager.cpp
        PARENT_SCOPE
        )