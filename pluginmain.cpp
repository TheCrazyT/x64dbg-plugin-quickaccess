#include "pluginmain.h"

#include <time.h>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QWindow>
#include <QInputDialog>
#include <QSettings>
#include <QDialog>
#include <QTimer>
#include <QObject>
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
const int ME_WINDOW_WIDTH = 0x19223302;
const int ME_ITEM_SIZE = 0x19223303;

const char* REGISTRY_FOLDER = R"(HKEY_CURRENT_USER\SOFTWARE\CrazyT\quickaccess)";

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
    QSettings registry(REGISTRY_FOLDER, QSettings::NativeFormat);
    bool ok;
    switch(callbackInfo->hEntry) {
        case ME_QUICKACCESS:
            dbg("Menu clicked");
            show_qa_window();
            break;
        case ME_WINDOW_WIDTH:
            {
                int windowWidth = QInputDialog::getInt(NULL, QObject::tr(""),
                                            QObject::tr("Window width:"), qwin->getWindowWidth(),
                                            10, 5000,
                                            1, &ok);
                if(ok){
                    qwin->setWindowWidth(windowWidth);
                    registry.setValue("window_width",windowWidth);
                    registry.sync();
                }
            }
            break;
        case ME_ITEM_SIZE:
            {
                int itemSize = QInputDialog::getInt(NULL, QObject::tr(""),
                                            QObject::tr("Item size:"), qwin->getItemSize(),
                                            10, 5000,
                                            1, &ok);
                if(ok){
                    qwin->setItemSize(itemSize);
                    registry.setValue("item_size",itemSize);
                    registry.sync();
                }
            }
            break;
        default:
            dbgf("invalid entry: %016x\n", callbackInfo->hEntry);
            break;
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
    _plugin_menuaddseparator(hMenu);
    if(!_plugin_menuaddentry(hMenu, ME_ITEM_SIZE, "Set item size")){
        dbg("Error registering the \"Set item size\" menu!");
    }
    if(!_plugin_menuaddentry(hMenu, ME_WINDOW_WIDTH, "Set window width")){
        dbg("Error registering the \"Set window width\" menu!");
    }
    _plugin_menuentrysethotkey(pluginHandle, ME_QUICKACCESS, "Ctrl+3");

    executeOnGuiThreadAndWait([]{
        // Create the QuickAccess dialog
        QWidget* wnd = QApplication::activeWindow();
        if(wnd == NULL){
            foreach (QWidget *w, qApp->topLevelWidgets()){
				if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w)) {
					wnd = w;
					break;
				}
			}
        }
        dbg(wnd->objectName().toStdString().c_str());
        QMainWindow* mwnd = qobject_cast<QMainWindow*>(wnd);
        if(mwnd != nullptr){
            dbg("Mainwindow found.");
            uint itemSize;
            uint windowWidth;
            QSettings registry(REGISTRY_FOLDER, QSettings::NativeFormat);
            itemSize = registry.value("item_size",25).toUInt();
            windowWidth = registry.value("window_width",500).toUInt();
            qwin = new QuickAccess(mwnd, itemSize, windowWidth);
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
