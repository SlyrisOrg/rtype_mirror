set(SOURCES lua-test.cpp
        ${CMAKE_SOURCE_DIR}/Project/rtype/utils/AnimatedSprite.cpp
        ${CMAKE_SOURCE_DIR}/Project/rtype/utils/Animation.cpp)

add_test_core(luaTest lua-test "${SOURCES}")
target_include_directories(lua-test PRIVATE
        "${CMAKE_SOURCE_DIR}/Project"
        "${CMAKE_SOURCE_DIR}/vendor/sol2"
        "${LUA_INCLUDE_DIR}"
        "${CMAKE_SOURCE_DIR}/vendor/rapidjson/include"
        "${SFML_INCLUDE_DIR}"
        )
target_link_libraries(lua-test core-utils core-log core-meta Boost::filesystem ${LUA_LIBRARIES} ${SFML_LIBRARIES})
if (WIN32)
    add_custom_command(TARGET lua-test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/assets ${CMAKE_SOURCE_DIR}/build-${CMAKE_BUILD_TYPE}/test/lua/assets)
else ()
    add_custom_command(TARGET lua-test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/assets ${CMAKE_SOURCE_DIR}/gcc-${CMAKE_BUILD_TYPE}-build/test/lua/assets)
endif ()