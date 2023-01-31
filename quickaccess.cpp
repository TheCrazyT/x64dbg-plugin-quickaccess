#include "quickaccess.h"
#include "pluginmain.h"

#include <QAction>
#include <QString>
#include <QGridLayout>
#include <QStringListModel>
#include <QCompleter>
#include <QListView>

static void parseMenu(QAction* menuAction, int level, QStringList& list, QString parentName, boolean addParentPath) {
    QMenu* menu = menuAction->menu();
    if(menu != nullptr){
        foreach(QAction* action, menu->actions()) {
            if(action != nullptr){
                dbgf("%0*d%s\n", level, 0, action->text().toStdString().c_str());
				parentName = menuAction->text();
                parseMenu(action, level+1, list, parentName, addParentPath);
            }
        }
    }else{
		char buf[1024];
		if(addParentPath){
			sprintf(&buf[0],"%s -> %s", parentName.replace("&","").toStdString().c_str(), menuAction->text().replace("&","").toStdString().c_str());
		}else{
			sprintf(&buf[0],"%s", menuAction->text().replace("&","").toStdString().c_str());
		}
        list.append(buf);
    }
}

static void parseWidget(QMainWindow* mwnd, QStringList& actionList) {
	foreach(QWidget* widget, mwnd->findChildren<QWidget*>(QString(), Qt::FindChildrenRecursively)) {
		if(widget != mwnd->menuBar()) {
			foreach(QAction* action, widget->actions()) {
				dbg(action->text().toStdString().c_str());
				QString parentName = widget->windowTitle();
				parseMenu(action, 1, actionList, parentName, true);
			}
		}
	}
}

QuickAccess::QuickAccess(QMainWindow* mwnd, uint itemSize, uint windowWidth) : QDialog(mwnd){
    this->setWindowWidth(windowWidth);
    this->setItemSize(itemSize);

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
            parseMenu(action, 1, actionList, QString(), false);
        }
        foreach (QAction* action, mwnd->actions()){
            dbg(action->text().toStdString().c_str());
            parseMenu(action, 1, actionList, QString(), false);
        }
		parseWidget(mwnd, actionList);
    }

    auto completer = new QCompleter(txt);
    completerEntries->setStringList(actionList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(completerEntries);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setFilterMode(Qt::MatchContains);

    completer->setMaxVisibleItems(items);
    QListView* popup = static_cast<QListView*>(completer->popup());
    popup->setMinimumWidth(width);

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
			QString find = txt.toLower();
			if(find.contains("->")){
				find = find.mid(find.indexOf(" -> ")+4);
			}
            if(menuAction->text().replace("&","").toLower().contains(find)) {
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
			foreach(QWidget* widget, ((QMainWindow*)parent())->findChildren<QWidget*>(QString(), Qt::FindChildrenRecursively)) {
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
