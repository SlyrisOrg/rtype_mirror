set(SOURCES components-test.cpp)

add_test_core(componentsTest components-test ${SOURCES})
target_include_directories(components-test PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include
        ${CMAKE_SOURCE_DIR}/Project/)
target_link_libraries(components-test ${SFML_LIBRARIES} core-log core-utils core-meta)
