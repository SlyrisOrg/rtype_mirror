set(SOURCES ecs-test.cpp)

add_test_core(ecsTest ecs-test ${SOURCES})
target_include_directories(ecs-test PRIVATE ${CMAKE_SOURCE_DIR}/Project)
target_link_libraries(ecs-test core-utils core-meta)
