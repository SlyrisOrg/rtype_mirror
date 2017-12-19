#!/bin/bash

PROGNAME=$0

CCOMPILER="gcc"
CXXCOMPILER="g++"
TYPE="Debug"

function parse_args()
{
    while getopts ":c:x:t:" opt; do
	case $opt in
	    c)
		CXXCOMPILER=$(which $OPTARG)
		;;
	    x)
		CCOMPILER=$(which $OPTARG)
		;;
	    t)
		TYPE=$OPTARG
		;;
	    h)
		echo "Usage: $PROGNAME [-c <compiler>] [-t <Debug | Release>]"
		exit 0
		;;
	    \?)
		echo "Invalid argument -$OPTARG, see $PROGNAME -h"
		;;
	esac
    done

    shift $(expr $OPTIND - 1)
    for t in "$@"; do
	TARGETS+="$t "
    done
}

function print_settings()
{
    echo "C compiler: $CCOMPILER"
    echo "C++ compiler: $CXXCOMPILER"
    echo "Build type: $TYPE"
}

function build_targets
{
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake -DCMAKE_BUILD_TYPE=$TYPE -DCMAKE_C_COMPILER=$CCOMPILER -DCMAKE_CXX_COMPILER=$CXXCOMPILER ..
    make -j2
    cd ..
}

function run_ctest
{
    cd $BUILD_DIR
    ctest --no-compress-output -T Test -D ExperimentalMemCheck
    cd ..
}

function run_xunit
{
    cd bin
    for i in *-test; do
	./$i --gtest_output="xml:${i}-${TYPE}-result.xml"
    done
    cd ..
    mkdir -p test-result
    cd test-result
    mkdir -p ctest-${TYPE}
    cd -
    cp bin/*.xml test-result/
    cp $BUILD_DIR/Testing/*/*.xml test-result/ctest/
}

parse_args "$@"
BUILD_DIR=${CCOMPILER}-${TYPE}-build
print_settings
build_targets
run_ctest
run_xunit
