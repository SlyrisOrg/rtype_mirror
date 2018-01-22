cmake_minimum_required(VERSION 3.6)
project(rtype)

enable_testing()
include(CTest)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BUILD_CORE_TESTS OFF CACHE BOOL "disable core tests" FORCE)
add_subdirectory(vendor/core)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/vendor/core/cmake-utils/Module)
set(CMAKE_CONFIG_PATH ${CMAKE_SOURCE_DIR}/vendor/core/cmake-utils)
include(${CMAKE_CONFIG_PATH}/include.cmake)
STANDARD_COMPILATION_SETTINGS()
RTYPE_CONFIG()
find_package(Lua 5.3)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_subdirectory(Project/rtype/core)
add_subdirectory(Project/rtype/scenes)
add_subdirectory(Project/rtype/systems)
add_subdirectory(Project/rtype/utils)
add_subdirectory(Project/rtype/config)
add_subdirectory(Project/rtype/audio)
add_subdirectory(Project/rtype/api)
add_subdirectory(Project/rtype/error)
add_subdirectory(Project/rtype/gutils)
add_subdirectory(Project/rtype/gui)
add_subdirectory(Project/rtype/video)
add_subdirectory(Project/rtype/protocol)
add_subdirectory(Project/rtype/netproto)
add_subdirectory(Project/rtype/entity)
add_subdirectory(Project/rtype/components)
add_subdirectory(Project/rtype/scenario)

set(HEADERS_PROJECT
        ${RTYPE_CORE_HEADERS}
        ${RTYPE_SCENES_HEADERS}
        ${RTYPE_SYSTEMS_HEADERS}
        ${RTYPE_UTILS_HEADERS}
        ${RTYPE_CONFIG_HEADERS}
        ${RTYPE_AUDIO_HEADERS}
        ${RTYPE_API_HEADERS}
        ${RTYPE_ERROR_HEADERS}
        ${RTYPE_GUTILS_HEADERS}
        ${RTYPE_GUI_HEADERS}
        ${RTYPE_PROTO_HEADERS}
        ${RTYPE_NETPROTO_HEADERS}
        ${RTYPE_ENTITY_HEADERS}
        ${RTYPE_VIDEO_HEADERS}
        ${RTYPE_COMPONENTS_HEADERS}
        ${RTYPE_SCENARIO_HEADERS}
        )

set(SOURCES_PROJECT Project/main.cpp
        ${RTYPE_CORE_SOURCES}
        ${RTYPE_UTILS_SOURCES}
        ${RTYPE_CONFIG_SOURCES}
        ${RTYPE_SCENES_SOURCES}
        ${RTYPE_AUDIO_SOURCES}
        ${RTYPE_API_SOURCES}
        ${RTYPE_ERROR_SOURCES}
        ${RTYPE_GUI_SOURCES}
        ${RTYPE_VIDEO_SOURCES}
        ${RTYPE_ENTITY_SOURCES}
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
        PUBLIC core-utils core-log core-net core-serialization
        PRIVATE ${SFML_LIBRARIES} ${LIBS_CEGUI} cpprestsdk::cpprest ${SFEMOVIE_LIBRARIES} Boost::boost)

if (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-unused-function")
endif (UNIX)

if (WIN32)
    source_group("rtype\\audio" FILES ${RTYPE_AUDIO_HEADERS} ${RTYPE_AUDIO_SOURCES})
    source_group("rtype\\api" FILES ${RTYPE_API_HEADERS} ${RTYPE_API_SOURCES})
    source_group("rtype\\config" FILES ${RTYPE_CONFIG_HEADERS} ${RTYPE_CONFIG_SOURCES})
    source_group("rtype\\core" FILES ${RTYPE_CORE_HEADERS} ${RTYPE_CORE_SOURCES})
    source_group("rtype\\error" FILES ${RTYPE_ERROR_HEADERS} ${RTYPE_ERROR_SOURCES})
    source_group("rtype\\gutils" FILES ${RTYPE_GUTILS_HEADERS})
    source_group("rtype\\gui" FILES ${RTYPE_GUI_HEADERS} ${RTYPE_GUI_SOURCES})
    source_group("rtype\\scenes" FILES ${RTYPE_SCENES_HEADERS} ${RTYPE_SCENES_SOURCES})
    source_group("rtype\\systems" FILES ${RTYPE_SYSTEMS_HEADERS})
    source_group("rtype\\utils" FILES ${RTYPE_UTILS_HEADERS} ${RTYPE_UTILS_SOURCES})
    source_group("rtype\\video" FILES ${RTYPE_VIDEO_HEADERS} ${RTYPE_VIDEO_SOURCES})
    if (CMAKE_BUILD_TYPE MATCHES Release)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
        target_compile_definitions(${PROJECT_NAME} PRIVATE
                _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
                _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
                WIN32_LEAN_AND_MEAN
                LOGGER_DISABLE_ALL)
    elseif (CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
        target_compile_definitions(${PROJECT_NAME} PRIVATE
                _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
                _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
                WIN32_LEAN_AND_MEAN
                WINDOWS_STANDARD_MAIN)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
            $ENV{SFML_ROOT}/lib/pdb ${CMAKE_SOURCE_DIR}/bin)
    endif ()
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4715")
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif (WIN32)

add_subdirectory(test)