#!/usr/bin/env bash

function build()
{
    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 15 2017 Win64" ..
    cmake --build . --config Debug --target game-template
}

build