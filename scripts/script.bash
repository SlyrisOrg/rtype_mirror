#!/bin/bash

RACINE_DIRECTORY=$PWD

function remove_old_build()
{
    cd ${RACINE_DIRECTORY}
    rm -rf DebugBuild
}

function prepare_build()
{
    remove_old_build

    mkdir DebugBuild
    cd ${RACINE_DIRECTORY}/DebugBuild
    cmake ../ -DCMAKE_BUILD_TYPE=Debug
    make -j
    cd ${RACINE_DIRECTORY}
}

function run_ctest()
{
    cd ${RACINE_DIRECTORY}/DebugBuild
    ctest --no-compress-output -T Test -D ExperimentalMemCheck || /usr/bin/true
    cd ${RACINE_DIRECTORY}
}

function run_xunit()
{
    cd ${RACINE_DIRECTORY}/bin
    for i in *-test
    do
	./$i --gtest_output="xml:$i-result.xml"
    done
    cd ${RACINE_DIRECTORY}
    mkdir test-result
    cd test-result && mkdir ctest && cd -
    cp bin/*.xml test-result/
    cp DebugBuild/Testing/*/*.xml test-result/ctest
}

echo ${RACINE_DIRECTORY}
rm -rf test-result
prepare_build
run_ctest
run_xunit
remove_old_build