#include "pluginmain.h"

#include <time.h>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QWindow>
#include <QDialog>
#include <QTimer>
#include <QWindowList>
#include <QMenuBar>
#include <QLineEdit>
#include <QAction>
#include <QObject>
#include <QGridLayout>
#include <QCompleter>
#include <QStringListModel>
#include <QThread>
#include <qlist.h>
#include <pluginsdk/_plugins.h>
#include <string.h>
#include <stdbool.h>

#include "quickaccess.h"

#define plugin_name "QuickAccess"
#define plugin_version 001

int pluginHandle;
int hMenu;

QuickAccess* qwin = nullptr;

const int ME_QUICKACCESS = 0x19223301;

static void executeOnGuiThreadAndWait(void(*worker)()){
    struct Context
    {
        HANDLE event;
        void(*worker)();
    };
    auto context = Context{ CreateEventW(nullptr, true, false, nullptr), worker };
    GuiExecuteOnGuiThreadEx([](void* data){
        auto context = (Context*)data;
        context->worker();
        SetEvent(context->event);
    }, &context);
    WaitForSingleObject(context.event, INFINITE);
    CloseHandle(context.event);
}

static void show_qa_window(){
    qwin->display();
}

static bool cb_plugin_command(int argc, char** argv){
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    GuiExecuteOnGuiThread(show_qa_window);
    return true;
}

static void cb_plugin_menuentry(CBTYPE cbType, void* info){
    Q_UNUSED(cbType);
    PLUG_CB_MENUENTRY* callbackInfo = (PLUG_CB_MENUENTRY*)info;
    if(callbackInfo->hEntry == ME_QUICKACCESS){
        dbg("Menu clicked");
        show_qa_window();
    }else{
        dbgf("Entry: %016x != %16x\n", callbackInfo->hEntry, ME_QUICKACCESS);
    }
}

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct){
    initStruct->pluginVersion = plugin_version;
    initStruct->sdkVersion = PLUG_SDKVERSION;
    strcpy(initStruct->pluginName, plugin_name);
    pluginHandle = initStruct->pluginHandle;

    dbgf("ME_QUICKACCESS: %016x\n",ME_QUICKACCESS);
    _plugin_registercallback(pluginHandle, CB_MENUENTRY, &cb_plugin_menuentry);
    _plugin_registercommand(pluginHandle, "quickaccess", &cb_plugin_command, false);

    dbg("plugin quickaccess loaded");
    return true;
}

DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct){
    hMenu = setupStruct->hMenu;
    if(!_plugin_menuaddentry(hMenu, ME_QUICKACCESS, "QuickAccess")){
        dbg("Error registering the \"quickaccess\" menu!");
    }
    _plugin_menuentrysethotkey(pluginHandle, ME_QUICKACCESS, "Ctrl+3");

    executeOnGuiThreadAndWait([]{
        // Create the QuickAccess dialog
        QWidget* wnd = QApplication::activeWindow();
        dbg(wnd->objectName().toStdString().c_str());
        QMainWindow* mwnd = qobject_cast<QMainWindow*>(wnd);
        if(mwnd != nullptr){
            dbg("Mainwindow found.");
            qwin = new QuickAccess(mwnd);
        }
    });

    dbg("plugsetup done");
}

DLL_EXPORT bool plugstop(){
    executeOnGuiThreadAndWait([]{
        if(qwin != nullptr){
            qwin->close();
            delete qwin;
        }
    });
    return true;
}
