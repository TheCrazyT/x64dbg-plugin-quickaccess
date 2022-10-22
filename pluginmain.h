#pragma once

#include <Windows.h>
#include <pluginsdk/_plugins.h>

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif //DLL_EXPORT

#ifdef DEBUG
    #define dbg _plugin_logputs
    #define dbgf _plugin_logprintf
#else
    #define dbg
    #define dbgf
#endif

//superglobal variables
extern int pluginHandle;
extern int hMenu;

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct);
DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct);
DLL_EXPORT bool plugstop();

#ifdef __cplusplus
}
#endif
