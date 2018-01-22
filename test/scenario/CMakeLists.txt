set(SOURCES scenario-test.cpp)

add_test_core(scenarioTest scenario-test ${SOURCES})
target_include_directories(scenario-test PRIVATE ${CMAKE_SOURCE_DIR}/Project ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include
        $ENV{SFEMOVIE_ROOT}/include ${SFML_INCLUDE_DIR})
target_link_libraries(scenario-test core-utils core-log core-serialization core-meta ${SFML_LIBRARIES} ${SFEMOVIE_LIBRARIES})
if(WIN32)
    add_custom_command(TARGET scenario-test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/assets ${CMAKE_SOURCE_DIR}/build-${CMAKE_BUILD_TYPE}/test/scenario/assets)
else()
    add_custom_command(TARGET scenario-test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/assets ${CMAKE_SOURCE_DIR}/gcc-${CMAKE_BUILD_TYPE}-build/test/scenario/assets)
endif()