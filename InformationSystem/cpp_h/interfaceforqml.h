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

    YouTube *api;
    QJsonObject *par;
    InterfaceForQml *obj;
    QJsonArray *result;
    QFutureWatcher<QJsonArray> futWat;

public slots:
    void sendResult(QJsonArray data);
    void getData(const QJsonObject &params);

signals:
    void ytData(const QJsonArray &data);
};

#endif // INTERFACEFORQML_H
