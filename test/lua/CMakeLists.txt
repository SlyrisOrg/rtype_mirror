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
