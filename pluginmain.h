#ifndef _PLUGINMAIN_H
#define _PLUGINMAIN_H

#include <windows.h>
#include <QDialog>
#include <QCompleter>
#include <QLineEdit>
#include <QMenuBar>
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




extern QDialog qwin;
extern QCompleter completer2;
extern QLineEdit txt;
extern QMenuBar* menuBar;
//superglobal variables
extern int pluginHandle;
extern HWND hwndDlg;
extern int hMenu;

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct);
DLL_EXPORT bool plugstop();
DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct);

#ifdef __cplusplus
}
#endif

#endif //_PLUGINMAIN_H
