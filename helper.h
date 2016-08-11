#ifndef HELPER_H
#define HELPER_H

#include <QObject>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = Q_NULLPTR);

signals:

public slots:

    void txtReturnPressed();
};

#endif // HELPER_H
