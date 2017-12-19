//
// Created by roman sztergbaum on 15/09/2017.
//

#include <iostream>
#include <rtype/config/Configuration.hpp>
#include <rtype/core/Game.hpp>

#if defined(USING_WINDOWS) && defined(WINDOWS_STANDARD_MAIN)
int main()
#elif defined(USING_WINDOWS) && !defined(WINDOWS_STANDARD_MAIN)
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
#include <cstdlib> //! getenv()

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv, char **env)
#endif
{
#ifdef USING_UNIX
    if (env == nullptr || getenv("DISPLAY") == nullptr)
        return 1;
#endif

#ifdef USING_WINDOWS
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif
    try {
        rtype::Configuration::contextSettings();
        rtype::Game rtype;
        rtype.run();
    }
    catch (const std::exception &error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    return 0;
}
