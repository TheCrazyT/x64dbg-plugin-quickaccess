#pragma once

#include <QDialog>
#include <QMainWindow>
#include <QLineEdit>
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
    }

    void setItemSize(uint value){
        m_itemSize = value;
    }

private slots:
    void txtReturnPressed();

private:
    QLineEdit* txt = nullptr;
    QMenuBar* menuBar = nullptr;
    uint m_windowWidth;
    uint m_itemSize;
};
