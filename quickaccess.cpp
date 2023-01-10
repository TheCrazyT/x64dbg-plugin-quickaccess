#include "quickaccess.h"
#include "pluginmain.h"

#include <QAction>
#include <QString>
#include <QGridLayout>
#include <QStringListModel>
#include <QCompleter>

static void parseMenu(QAction* menuAction, int level, QStringList& list) {
    QMenu* menu = menuAction->menu();
    if(menu != nullptr){
        foreach(QAction* action, menu->actions()) {
            if(action != nullptr){
                dbgf("%0*d%s\n", level, 0, action->text().toStdString().c_str());
                parseMenu(action, level+1, list);
            }
        }
    }else{
        list.append(menuAction->text().replace("&",""));
    }
}

static void parseWidget(QWidget* mainWidget,QStringList& actionList) {
	foreach(QWidget* widget, mainWidget->findChildren<QWidget*>(QString(), Qt::FindChildrenRecursively)) {
		foreach(QAction* action, widget->actions()) {
			dbg(action->text().toStdString().c_str());
			parseMenu(action, 1, actionList);
		}
		parseWidget(widget, actionList);
	}
}

QuickAccess::QuickAccess(QMainWindow* mwnd) : QDialog(mwnd){
    txt = new QLineEdit();
    auto layout = new QGridLayout();
    layout->addWidget(txt->window(), 0, 0);

    connect(txt, SIGNAL(returnPressed()), this, SLOT(txtReturnPressed()));

    setBaseSize(200, 100);
    setLayout(layout);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);

    QStringList actionList;
    auto completerEntries = new QStringListModel(this);
    menuBar = mwnd->menuBar();
    if(menuBar != nullptr){
        dbg("Mainwindow valid.");
        foreach (QAction* action, menuBar->actions()){
            dbg(action->text().toStdString().c_str());
            parseMenu(action, 1, actionList);
        }
        foreach (QAction* action, mwnd->actions()){
            dbg(action->text().toStdString().c_str());
            parseMenu(action, 1, actionList);
        }
		parseWidget(mwnd, actionList);
    }

    auto completer = new QCompleter(txt);
    completerEntries->setStringList(actionList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(completerEntries);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setFilterMode(Qt::MatchContains);

    txt->setCompleter(completer);
}

void QuickAccess::display() {
    show();
    raise();
    activateWindow();

    txt->setVisible(true);
    txt->setEnabled(true);
    txt->setFocus();
}

bool findByString(QString txt, QAction* menuAction){
        QMenu* menu = menuAction->menu();
        if(menu != NULL){
            foreach(QAction* action, menu->actions()) {
                if(action != NULL){
                    if(findByString(txt, action)) {
                        return true;
                    }
                }
            }
        }else{
            if(menuAction->text().replace("&","").toLower().contains(txt.toLower())) {
                emit menuAction->trigger();
                return true;
            }
        }
        return false;
}

void QuickAccess::txtReturnPressed() {
    dbg("return pressed");
	boolean found = false;
    if(menuBar != NULL){
        dbg("Mainwindow valid.");
        foreach(QAction* action, menuBar->actions()){
           if(findByString(txt->text(), action)){
			   found = true;
               break;
           }
        }
    }
	if((!found)&&(parent() != NULL)) {
		foreach(QAction* action, ((QMainWindow*)parent())->actions()) {
		   if(findByString(txt->text(), action)){
			   found = true;
			   break;
		   }
		}
		if(!found) {
			foreach(QWidget* widget, findChildren<QWidget*>(QString(), Qt::FindChildrenRecursively)) {
				foreach(QAction* action, widget->actions()){
				   if(findByString(txt->text(), action)){
					   found = true;
					   break;
				   }
				}
			}
		}
	}
    hide();
    accept();
}
