#!/bin/bash

GCC="TRUE"
CLANG="FALSE"
DEBUG="TRUE"
RELEASE="FALSE"
THREAD_SANITIZER="FALSE"
MEMORY_SANITIZER="FALSE"

function help
{
    echo "./build.sh [-gcc, --clang, --with_tsan, --with-asan, --debug, --release]"
    exit 1
}

function parse_argument()
{
    for i in "$@"
    do
    case $i in
        --gcc)
        GCC="TRUE"
        ;;
        --clang)
        CLANG="TRUE"
        GCC="FALSE"
        ;;
        --with_tsan)
        THREAD_SANITIZER="TRUE"
        ;;
        --with_asan)
        MEMORY_SANITIZER="TRUE"
        ;;
        --debug_and_release)
        DEBUG="TRUE"
        RELEASE="TRUE"
        ;;
        --debug)
        DEBUG="TRUE"
        RELEASE="FALSE"
        ;;
        --release)
        RELEASE="TRUE"
        DEBUG="FALSE"
        ;;
        --default)
        GCC="TRUE"
        DEBUG="TRUE"
        ;;
        *)
        help        # unknown option
        ;;
    esac
    done
}

function run_ctest
{
    cd $1
    ctest --no-compress-output -T Test -D ExperimentalMemCheck
    cd ..
}

function run_xunit
{
    cd bin
    for i in *-test
    do
	./$i --gtest_output="xml:$i-$1-result.xml"
    done
    cd ..
    mkdir -p test-result
    cd test-result
    mkdir -p ctest-$1
    cd -
    cp bin/*.xml test-result/
    if [[ ${THREAD_SANITIZER} == "FALSE" && ${MEMORY_SANITIZER} == "FALSE" ]]
    then
        cp $1/Testing/*/*.xml test-result/ctest-$1
    fi
}

function run_test
{
    if [[ ${THREAD_SANITIZER} == "FALSE" && ${MEMORY_SANITIZER} == "FALSE" ]]
    then
        run_ctest $1
    fi
    run_xunit $1
}

function run()
{
    rm -rf $1
    echo "1 -> $1"
    echo "2 -> $2"
    echo "3 -> $3"
    mkdir -p $1 && cd $1 && cmake $2 ../ && $3
    cd ..
    run_test "$1"
}

function uniform_build()
{
    CMD="-DCMAKE_BUILD_TYPE=${1} -DCMAKE_C_COMPILER=/usr/bin/${2} -DCMAKE_CXX_COMPILER=/usr/bin/${3}"
    BUILD="make -j2"
    TYPE="${4}-build"
    if [[ ${THREAD_SANITIZER} == "TRUE" ]]
    then
        CMD_THR="$CMD -DCMAKE_CXX_FLAGS_${5}=-fsanitize=thread CMAKE_LINKER_FLAGS_${5}=-fsanitize=thread"
        TYPE_THR="${TYPE}-tsan"
        run "${TYPE_THR}" "${CMD_THR}" "${BUILD}"
    fi

    if [[ ${MEMORY_SANITIZER} == "TRUE" ]]
    then
        CMD_MEMORY="$CMD -DCMAKE_CXX_FLAGS_${5}=\"-fno-omit-frame-pointer -fsanitize=address\" CMAKE_LINKER_FLAGS_${5}=\"-fsanitize=address -fno-omit-frame-pointer\""
        TYPE_MEMORY="${TYPE}-asan"
        run "${TYPE_MEMORY}" "${CMD_MEMORY}" "${BUILD}"
    fi

    if [[ ${THREAD_SANITIZER} == "FALSE" && ${MEMORY_SANITIZER} == "FALSE" ]]
    then
        run "${TYPE}" "${CMD}" "${BUILD}"
    fi
}

function debug_gcc_build()
{
     uniform_build "Debug" "gcc" "g++" "gcc-debug" "DEBUG"
}

function release_gcc_build()
{
     uniform_build "Release" "gcc" "g++" "gcc-release" "RELEASE"
}


function gcc_build()
{
    echo "gcc build trigger"
    if [[ ${DEBUG} == "TRUE" ]]
    then
        debug_gcc_build
    fi

    if [[ ${RELEASE} == "TRUE" ]]
    then
        release_gcc_build
    fi
}

function debug_clang_build()
{
     uniform_build "Debug" "clang-5.0" "clang++-5.0" "clang-debug" "DEBUG"
}

function release_clang_build()
{
    uniform_build "Release" "clang-5.0" "clang++-5.0" "clang-release" "RELEASE"
}

function clang_build()
{
    echo "clang build trigger"
    if [[ ${DEBUG} == "TRUE" ]]
    then
        debug_clang_build
    fi

    if [[ ${RELEASE} == "TRUE" ]]
    then
        release_clang_build
    fi
}

function call_fn()
{
    if [[ ${GCC} == "TRUE" ]];
    then
        gcc_build
    fi

    if [[ ${CLANG} == "TRUE" ]]
    then
        clang_build
    fi
}

parse_argument $@
echo "--------------------------------------------"
echo "GCC=${GCC}"
echo "CLANG=${CLANG}"
echo "THREAD_SANITIZER=${THREAD_SANITIZER}"
echo "MEMORY_SANITIZER=${MEMORY_SANITIZER}"
echo "DEBUG=${DEBUG}"
echo "RELEASE=${RELEASE}"
echo "--------------------------------------------"
call_fn
