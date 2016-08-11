#include "helper.h"
#include "pluginmain.h"
#include <QAction>
#include <QString>
Helper::Helper(QObject *parent) : QObject(parent)
{

}
bool findByString(QString txt,QAction* action){
        QMenu* menu = action->menu();
        if(menu != NULL){
            for(QAction* action2 : menu->actions()){
                if(action2 != NULL){
                    if(findByString(txt,action2)){
                        return true;
                    }
                }
            }
        }else{
            if(action->text().replace("&","").compare(txt)==0){
                emit action->trigger();
                return true;
            }
        }
        return false;
}

void Helper::txtReturnPressed(){
    dbg("return pressed");

    if(menuBar != NULL){
        dbg("Mainwindow valid.");
        for(QAction* action : menuBar->actions()){
           if(findByString(txt.text(),action)){
               break;
           }
        }
    }

    qwin.hide();
}
