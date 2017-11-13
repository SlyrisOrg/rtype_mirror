#!/bin/bash

function install_cegui()
{
    echo "Installation de CEGUI ...";
    echo "Installation des Dependances de CEGUI ..."
    read -p "Do you want install cegui dependancies (y/n) y by default?" choice
    case "$choice" in
        y|Y ) INSTALL_CEGUI_DEPENDANCIES="True";;
        n|N ) INSTALL_CEGUI_DEPENDANCIES="False";;
        * ) INSTALL_CEGUI_DEPENDANCIES="True";;
    esac
    if [[ $INSTALL_CEGUI_DEPENDANCIES == "True" ]];
    then
        echo "Cegui installing dependancies ..."
        apt-get install libfreeimage-dev libiconv-hook-dev libfribidi-dev libpcre3-dev libfreetype6-dev freeglut3-dev libpython-dev libglew-dev libglm-dev libois-dev libxml2-dev libboost-dev mercurial -y
    fi
    hg clone https://bitbucket.org/cegui/cegui
    cd cegui && hg update v0-8 && hg checkout 75b9fcc4e3cb
    mkdir release && cd release
    cmake -DCMAKE_BUILD_TYPE=Debug -DCEGUI_BUILD_PYTHON_MODULES=OFF -DCEGUI_BUILD_RENDERER_OPENGL=ON -DCEGUI_BUILD_RENDERER_OPENGL3=ON ../
    make install
    if (( $? == 0));
    then
        echo "Cegui is installed with any problem."
    fi
}

function install_boost()
{
    echo "Installing boost ..."
    apt-get install libboost-all-dev
}

function install_cmake()
{
    apt-get remove cmake -y
    apt-get purge cmake -y
    wget https://cmake.org/files/v3.10/cmake-3.10.0-rc4.tar.gz
    tar xf cmake-3.10.0-rc4.tar.gz
    cd cmake-3.10.0-rc4
    ./configure
    make install
}

function install_sfemovie()
{
    apt-get install yasm libsfml-dev -y
    git clone https://github.com/Yalir/sfeMovie.git
    cd sfeMovie && mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make install
}

function install_cpprest
{
    apt-get install libcpprest-dev -y
}

function install()
{
    read -p "Do you want install cmake 3.10.rc3, this will purge your old version of cmake (y/n) y by default ? " choice
    case "$choice" in
        y|Y ) INSTALL_CMAKE="True";;
        n|N ) INSTALL_CMAKE="False";;
        * ) INSTALL_CMAKE="True";;
    esac

    if [[ $INSTALL_CMAKE == "True" ]];
    then
        install_cmake
    fi

    read -p "Do you want install libboost-all-dev (y/n) y by default ? " choice
    case "$choice" in
        y|Y ) INSTALL_BOOST="True";;
        n|N ) INSTALL_BOOST="False";;
        * ) INSTALL_BOOST="True";;
    esac

    if [[ $INSTALL_BOOST == "True" ]];
    then
        install_boost
    fi

    read -p "Do you want install cegui (y/n) y by default?" choice
    case "$choice" in
        y|Y ) INSTALL_CEGUI="True";;
        n|N ) INSTALL_CEGUI="False";;
        * ) INSTALL_CEGUI="True";;
    esac

    if [[ $INSTALL_CEGUI == "True" ]];
    then
        install_cegui
    fi

    read -p "Do you want install sfemovie (y/n) y by default?" choice
    case "$choice" in
        y|Y ) INSTALL_SFEMOVIE="True";;
        n|N ) INSTALL_SFEMOVIE="False";;
        * ) INSTALL_SFEMOVIE="True";;
    esac

    if [[ $INSTALL_SFEMOVIE == "True" ]];
    then
        install_sfemovie
    fi

    read -p "Do you want install cpprest (y/n) y by default?" choice
    case "$choice" in
        y|Y ) INSTALL_CPPREST="True";;
        n|N ) INSTALL_CPPREST="False";;
        * ) INSTALL_CPPREST="True";;
    esac

    if [[ $INSTALL_CPPREST == "True" ]];
    then
        install_cpprest
    fi
}

function clean_tmp()
{
    cd /tmp
    rm -rf cegui
    rm -rf sfeMovie
}

function prerequisite()
{
    clean_tmp
    apt-get update -y
    apt-get upgrade -y
}

prerequisite
install
