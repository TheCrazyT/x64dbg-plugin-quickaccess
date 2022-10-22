#pragma once

#include <QDialog>
#include <QMainWindow>
#include <QLineEdit>
#include <QMenuBar>

class QuickAccess : public QDialog{
    Q_OBJECT
public:
    explicit QuickAccess(QMainWindow* mwnd);
    void display();

private slots:
    void txtReturnPressed();

private:
    QLineEdit* txt = nullptr;
    QMenuBar* menuBar = nullptr;
};
