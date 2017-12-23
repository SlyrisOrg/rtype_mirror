set(SOURCES protocol-test.cpp)

add_test_core(protocolTest protocol-test ${SOURCES})
target_include_directories(protocol-test PRIVATE ${CMAKE_SOURCE_DIR}/Project/rtype/)
target_link_libraries(protocol-test core-utils core-serialization)
