set(SOURCES components-test.cpp)

add_test_core(componentsTest components-test ${SOURCES})
target_include_directories(components-test PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/rapidjson/include
        ${CMAKE_SOURCE_DIR}/vendor/sol2
        ${LUA_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/Project/
        $ENV{SFEMOVIE_ROOT}/include)
target_link_libraries(components-test ${SFML_LIBRARIES} core-log core-serialization core-utils core-meta ${LUA_LIBRARIES})
if (WIN32)
    add_custom_command(TARGET components-test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/assets ${CMAKE_SOURCE_DIR}/build-${CMAKE_BUILD_TYPE}/test/components/assets)
    target_compile_definitions(components-test PRIVATE
            _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
            _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
else ()
    add_custom_command(TARGET components-test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/assets ${CMAKE_SOURCE_DIR}/gcc-${CMAKE_BUILD_TYPE}-build/test/components/assets)
endif ()