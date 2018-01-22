set(SOURCES quadtree-test.cpp)

add_test_core(quadTreeTest quadTree-test ${SOURCES})
target_include_directories(quadTree-test PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include
        ${CMAKE_SOURCE_DIR}/Project/
        ${CMAKE_SOURCE_DIR}/vendor/sol2
        ${LUA_INCLUDE_DIR}
        $ENV{SFEMOVIE_ROOT}/include)
target_link_libraries(quadTree-test ${SFML_LIBRARIES} core-log core-utils core-meta)
