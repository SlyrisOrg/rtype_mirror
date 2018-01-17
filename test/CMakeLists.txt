########################################################################################################################
#                                Settings Google Test Framework                                                        #
#                                                                                                                      #
########################################################################################################################

cmake_minimum_required(VERSION 3.7)


########################################################################################################################

########################################################################################################################
#                               Force static on MSVC                                                                   #
#                                                                                                                      #
########################################################################################################################

if (MSVC)
    foreach (flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if (${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif (${flag_var} MATCHES "/MD")
        if (${flag_var} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif (${flag_var} MATCHES "/MDd")
    endforeach (flag_var)
endif (MSVC)

########################################################################################################################

include_directories(${CMAKE_SOURCE_DIR}/vendor/googletest/include/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor/googletest/googlemock/include)

########################################################################################################################
#                                           SETUP CTEST                                                                #
#                                                                                                                      #
########################################################################################################################

include(CTest)
set(CTEST_BINARY_DIRECTORY "/binary")
set(CTEST_SOURCE_DIRECTORY "/source")

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()


if (LINUX)
    set(CTEST_USE_LAUNCHERS 1)
    set(CTEST_COVERAGE_COMMAND "gcov")
    set(CTEST_MEMORYCHECK_COMMAND, "/usr/bin/valgrind")
    set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
    ##set(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/valgrind_suppress.txt")
endif (LINUX)

macro(add_test_core NAME_TEST NAME_EXECUTABLE SOURCE)
    message(STATUS "${NAME_TEST} ${NAME_EXECUTABLE} ${SOURCE}")
    add_executable(${NAME_EXECUTABLE} ${SOURCE})
    target_link_libraries(${NAME_EXECUTABLE} gtest gtest_main)
if(WIN32)
    add_custom_command(TARGET ${NAME_EXECUTABLE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE} ${CMAKE_SOURCE_DIR}/bin)
endif()
    add_test(NAME ${NAME_TEST} COMMAND $<TARGET_FILE:${NAME_EXECUTABLE}>)
endmacro()

########################################################################################################################
add_subdirectory(protocol)
add_subdirectory(ecs)
add_subdirectory(components)
add_subdirectory(quadtree)
