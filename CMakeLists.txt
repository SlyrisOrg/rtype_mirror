cmake_minimum_required(VERSION 3.6)
project(rtype)

enable_testing()
include(CTest)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(vendor/core)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/vendor/core/cmake-utils/Module)
set(CMAKE_CONFIG_PATH ${CMAKE_SOURCE_DIR}/vendor/core/cmake-utils)
include(${CMAKE_CONFIG_PATH}/include.cmake)
STANDARD_COMPILATION_SETTINGS()
GAME_TEMPLATE_CONFIG()

add_subdirectory(Project/rtype/core)
add_subdirectory(Project/rtype/scenes)
add_subdirectory(Project/rtype/systems)
add_subdirectory(Project/rtype/utils)
add_subdirectory(Project/rtype/config)
add_subdirectory(Project/rtype/audio)
add_subdirectory(Project/rtype/api)

set(HEADERS_PROJECT
        ${GAME_TEMPLATE_CORE_HEADERS}
        ${GAME_TEMPLATE_SCENES_HEADERS}
        ${GAME_TEMPLATE_SYSTEMS_HEADERS}
        ${GAME_TEMPLATE_UTILS_HEADERS}
        ${GAME_TEMPLATE_CONFIG_HEADERS}
        ${GAME_TEMPLATE_AUDIO_HEADERS}
        ${GAME_TEMPLATE_API_HEADERS}
        Project/rtype/error/RTypeError.hpp
        Project/rtype/gutils/Manager.hpp
        Project/rtype/gutils/gutils.hpp
        Project/rtype/gutils/Event/InsideEvents.hpp
        Project/rtype/gutils/Interface/AScene.hpp
        Project/rtype/gutils/Interface/ISystem.hpp
        Project/rtype/gutils/Manager/EventManager.hpp
        Project/rtype/gutils/Manager/SystemManager.hpp
        Project/rtype/gui/GuiManager.hpp
        Project/rtype/gui/BaseGui.hpp
        Project/rtype/gameobject/GameObject.hpp
        Project/rtype/video/VideoManager.hpp
        )

set(SOURCES_PROJECT Project/main.cpp
        ${GAME_TEMPLATE_CORE_SOURCES}
        ${GAME_TEMPLATE_UTILS_SOURCES}
        ${GAME_TEMPLATE_CONFIG_SOURCES}
        ${GAME_TEMPLATE_SCENES_SOURCES}
        ${GAME_TEMPLATE_AUDIO_SOURCES}
        ${GAME_TEMPLATE_API_SOURCES}
        Project/rtype/error/RTypeError.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES_PROJECT} ${HEADERS_PROJECT})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Project
        ${CEGUI_INCLUDE_DIR}
        ${SFML_INCLUDE_DIR}
        $ENV{SFEMOVIE_ROOT}/include
        ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC core-utils core-log core-net
        PRIVATE ${SFML_LIBRARIES} ${LIBS_CEGUI} cpprestsdk::cpprest ${SFEMOVIE_LIBRARIES})

if (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-unused-function")
endif (UNIX)

if (WIN32)
    source_group("rtype\\audio" FILES ${GAME_TEMPLATE_AUDIO_HEADERS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
            _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4715")
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif (WIN32)