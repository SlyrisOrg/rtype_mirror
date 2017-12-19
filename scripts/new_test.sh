#!/bin/bash

function err()
{
    echo "$@"
    exit 1
}

[ $# -ne 1 ] && err "Usage: $0 <testname>"
[ ! -d test ] && err "Use this script from the root of the repository"

TESTNAME="$1"
[ -d test/$TESTNAME ] && err "Refusing to overwrite existing test $TESTNAME"

echo "Creating new test $TESTNAME"
mkdir test/$TESTNAME

echo "set(SOURCES ${TESTNAME}-test.cpp)" > test/$TESTNAME/CMakeLists.txt
echo "" >> test/$TESTNAME/CMakeLists.txt
echo "add_test_core(${TESTNAME}Test ${TESTNAME}-test" '${SOURCES})' >> test/$TESTNAME/CMakeLists.txt

echo "add_subdirectory($TESTNAME)" >> test/CMakeLists.txt

echo "Test created in test/$TESTNAME"
echo "Implement it in test/${TESTNAME}-test.cpp"
