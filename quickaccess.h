#pragma once

#include <QDialog>
#include <QMainWindow>
#include <QLineEdit>
#include <QListView>
#include <QMenuBar>

class QuickAccess : public QDialog{
    Q_OBJECT
public:
    explicit QuickAccess(QMainWindow* mwnd, uint itemSize, uint windowWidth);
    void display();
    Q_PROPERTY(uint windowWidth READ getWindowWidth WRITE setWindowWidth)
    Q_PROPERTY(uint itemSize READ getItemSize WRITE setItemSize)

    void setWindowWidth(uint value){
        m_windowWidth = value;
        if(txt != NULL){
            auto completer = txt->getCompleter();
            QListView* popup = static_cast<QListView*>(completer->popup());
            popup->setMinimumWidth(value);
        }
    }

    void setItemSize(uint value){
        m_itemSize = value;
        if(txt != NULL){
            auto completer = txt->getCompleter();
            completer->setMaxVisibleItems(value);
        }
    }

    uint getWindowWidth(){
        return m_windowWidth;
    }

    uint getItemSize(){
        return m_itemSize;
    }

private slots:
    void txtReturnPressed();

private:
    QLineEdit* txt = nullptr;
    QMenuBar* menuBar = nullptr;
    uint m_windowWidth;
    uint m_itemSize;
};
