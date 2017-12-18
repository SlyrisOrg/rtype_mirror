set(SCENES ${CMAKE_SOURCE_DIR}/Project/rtype/scenes/)

set(RTYPE_SCENES_HEADERS
        ${SCENES}/IntroScene.hpp
        ${SCENES}/EnumScenes.hpp
        ${SCENES}/LoginScene.hpp
        ${SCENES}/ProfilScene.hpp
        ${SCENES}/CreateScene.hpp
        ${SCENES}/LoadingScene.hpp
        ${SCENES}/SceneManager.hpp
        ${SCENES}/private/GameSettings.hpp
        ${SCENES}/private/ForwardEnumProfil.hpp
        ${SCENES}/private/ForwardEnumCreate.hpp
        ${SCENES}/private/ForwardEnumLoading.hpp
        ${SCENES}/private/ForwardEnumLogin.hpp
        PARENT_SCOPE)

set(RTYPE_SCENES_SOURCES
        ${SCENES}/LoginScene.cpp
        ${SCENES}/CreateScene.cpp
        ${SCENES}/ProfilScene.cpp
        ${SCENES}/IntroScene.cpp
        PARENT_SCOPE
        )

if (WIN32)
    source_group("Header Files\\Scenes" FILES ${RTYPE_SCENES_HEADERS})
endif(WIN32)