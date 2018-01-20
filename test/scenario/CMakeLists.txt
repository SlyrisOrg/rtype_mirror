set(SOURCES scenario-test.cpp)

add_test_core(scenarioTest scenario-test ${SOURCES})
target_include_directories(scenario-test PRIVATE ${CMAKE_SOURCE_DIR}/Project ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include
        $ENV{SFEMOVIE_ROOT}/include ${SFML_INCLUDE_DIR})
target_link_libraries(scenario-test core-log ${SFML_LIBRARIES} ${SFEMOVIE_LIBRARIES})
