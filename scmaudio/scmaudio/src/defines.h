#pragma once

#ifdef SCMAUDIO_EXPORT
    #define SCMAUDIO_API __declspec(dllexport)
#else
    #define SCMAUDIO_API __declspec(dllimport)
#endif