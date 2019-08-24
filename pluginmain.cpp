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
#include <qlist.h>
#include <pluginsdk/_plugins.h>
#include <string.h>
#include <stdbool.h>

#include "helper.h"
#define plugin_name "QuickAccess"
#define plugin_version 001



QMenuBar* menuBar;
QDialog qwin;
int pluginHandle;
HWND hwndDlg;
int hMenu;
QLineEdit txt;
QTimer timer;
QGridLayout layout;
Helper helper;
QStringListModel completerEntries;
QStringList completerEntriesL;
QCompleter completer1;
void init(PLUG_INITSTRUCT* initStruct);

const int ME_QUICKACCESS = 0x19223301;

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct)
{
    initStruct->pluginVersion=plugin_version;
    initStruct->sdkVersion=PLUG_SDKVERSION;
    strcpy(initStruct->pluginName, plugin_name);
    pluginHandle=initStruct->pluginHandle;
	init(initStruct);
    dbg("plugin quickaccess loaded");
    return true;
}

DLL_EXPORT bool plugstop()
{
    return true;
}
void parseMenu(QAction* action,int level){
    QMenu* menu = action->menu();
    if(menu != NULL){
        for(QAction* action2 : menu->actions()){
            if(action2 != NULL){
                dbgf("%0*d%s\n",level,0,action2->text().toStdString().c_str());
                parseMenu(action2,level+1);
            }
        }
    }else{
        completerEntriesL.append(action->text().replace("&",""));
    }
}
DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct)
{
    hwndDlg = setupStruct->hwndDlg;
    hMenu = setupStruct->hMenu;
	if(!_plugin_menuaddentry(hMenu,ME_QUICKACCESS,"QuickAccess")){
        dbg("Error registering the \"quickaccess\" menu!");
	}
    dbg("plugsetup done");
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}

bool cb_plugin_command(
	int argc, //argument count (number of arguments + 1)
	char** argv //array of arguments (argv[0] is the full command, arguments start at argv[1])
)
{
	if(strcmp(argv[0],"quickaccess")==0){
		qwin.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		qwin.setModal(true);
		qwin.show();
    		qwin.raise();
    		qwin.activateWindow();
		txt.setVisible(true);
		txt.setEnabled(true);
		txt.setFocus();
	}
	return true;
}
void cb_plugin_menuentry(CBTYPE bType,void* info)
{
	PLUG_CB_MENUENTRY* callbackInfo = (PLUG_CB_MENUENTRY*)info;
	if(callbackInfo->hEntry == ME_QUICKACCESS){
		dbg("Menu clicked");
		qwin.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		qwin.show();
    		qwin.raise();
    		qwin.activateWindow();
		txt.setVisible(true);
		txt.setEnabled(true);
		txt.setFocus();
	}else{
       		dbgf("Entry: %016x != %16x\n",callbackInfo->hEntry,ME_QUICKACCESS);
	}
}
void cb_plugin_windows_event(CBTYPE bType,void* info)
{
	PLUG_CB_WINEVENTGLOBAL* callbackInfo = (PLUG_CB_WINEVENTGLOBAL*)info;
	MSG* msg = callbackInfo->message;
	switch(msg->message){
		case WM_KEYUP:
            dbgf("hwndDlg: %016x wnd: %016x\n",hwndDlg,msg->hwnd);
            dbgf("msg %016x,wparam %016x,lparam %016x\n",msg->message,msg->wParam,msg->lParam);
			break;
	}
}

void initMenu(){
    timer.stop();
    QWidget* wnd = QApplication::activeWindow();
    dbg(wnd->objectName().toStdString().c_str());
    QMainWindow* mwnd = qobject_cast<QMainWindow*>(wnd);
    if(mwnd != NULL){
        qwin.setBaseSize(200,100);
        qwin.setParent(mwnd);
        qwin.setLayout(layout.layout());
        helper.setParent(&txt);


        txt.connect(&txt, SIGNAL(returnPressed()), &qwin, SLOT(accept()));
        txt.connect(&txt, SIGNAL(returnPressed()), &helper, SLOT(txtReturnPressed()) );
        layout.addWidget(txt.window(),0,0);


        dbg("Mainwindow found.");
        menuBar = mwnd->menuBar();
        if(menuBar != NULL){
            dbg("Mainwindow valid.");
            for(QAction* action : menuBar->actions()){
                dbg(action->text().toStdString().c_str());
                parseMenu(action,1);
            }
        }
        completerEntries.setStringList(completerEntriesL);
        completer1.setCaseSensitivity(Qt::CaseInsensitive);
        completer1.setParent(txt.window());
        completer1.setModel(&completerEntries);
        completer1.setCompletionMode(QCompleter::PopupCompletion);

        txt.setCompleter(&completer1);
    }
}

void init(PLUG_INITSTRUCT* initStruct)
{
	dbgf("ME_QUICKACCESS: %016x\n",ME_QUICKACCESS);
	_plugin_registercallback(initStruct->pluginHandle,CB_MENUENTRY,&cb_plugin_menuentry);
	//_plugin_registercallback(initStruct->pluginHandle,CB_WINEVENTGLOBAL,&cb_plugin_windows_event);
	_plugin_registercommand(initStruct->pluginHandle,"quickaccess",&cb_plugin_command,false);
	timer.singleShot(500,&initMenu);
}
