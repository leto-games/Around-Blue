#pragma once

// ???????????????????????????
// #ifdef LETOCORE_EXPORT_LIBRARY
// #include <LetoAPI_V1/LetoAPI_V1_Version.h>
// #define LETO_API_V1_USE LETO_API_V1_MINOR
// #endif

#ifdef _WIN32
    #ifdef LETOGAMES_EXPORT_LIBRARY
        #define LETO_GAMES_EXPORT __declspec(dllexport)
    #else
        #define LETO_GAMES_EXPORT __declspec(dllimport)
    #endif
#else
    #define LETO_GAMES_EXPORT
#endif
