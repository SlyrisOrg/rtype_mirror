//
// Created by roman sztergbaum on 15/09/2017.
//

#include <iostream>
#include <rtype/config/Configuration.hpp>
#include <rtype/core/Game.hpp>

int main()
{
    try {
        rtype::Configuration::initialize();
        rtype::Game rtype;

        rtype.run();
    }
    catch (const std::exception &error) {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}