#pragma once

#if (defined(_WIN32) || defined(USE_SDL1))
    #ifdef _WIN32
        #ifdef _MSC_VER
            #ifdef USE_SDL1
                #pragma comment(lib, "SDL.lib")
            #else
                #pragma comment(lib, "SDL2.lib")
            #endif
        #endif
        #include <SDL.h>
        #include <SDL_syswm.h>
    #else
        #include <SDL/SDL.h>
    #endif

    #ifdef USE_SDL1
        // These macros aren't present in older versions of SDL 1.2.
        // In those versions, event.button.button will either never
        // equal these, having the effect of silently disabling scroll
        // wheel support, or it will and scrolling will work normally,
        // so it's safe to define them ourselves.
        #ifndef SDL_BUTTON_WHEELUP
        #define SDL_BUTTON_WHEELUP 4
        #endif
        #ifndef SDL_BUTTON_WHEELDOWN
        #define SDL_BUTTON_WHEELDOWN 5
        #endif
    #endif
#else
    #include <SDL2/SDL.h>
#endif
