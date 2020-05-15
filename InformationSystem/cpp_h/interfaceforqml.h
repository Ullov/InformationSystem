#ifndef INTERFACEFORQML_H
#define INTERFACEFORQML_H

#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "youtube.h"

class InterfaceForQml : public QObject
{
    Q_OBJECT
public:
    InterfaceForQml();

public slots:
    void getData(const QJsonObject &params);

private slots:
    void sendResult(const QJsonObject &params);

signals:
    void result(const QJsonObject &data);
};

#endif // INTERFACEFORQML_H
