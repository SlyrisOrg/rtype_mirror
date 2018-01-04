set(SOURCES quadtree-test.cpp)

add_test_core(quadTreeTest quadTree-test ${SOURCES})
target_include_directories(quadTree-test PRIVATE ${CMAKE_SOURCE_DIR}/Project)
target_link_libraries(quadTree-test ${SFML_LIBRARIES})
