#include "interfaceforqml.h"

#include <QFutureWatcher>

InterfaceForQml::InterfaceForQml() {}

void InterfaceForQml::getData(const QJsonObject &params)
{
    QThread *thread = new QThread();
    api = new YouTube();
    api->params = params;
    api->moveToThread(thread);
    connect(thread, SIGNAL(started()), api, SLOT(extractInfo()));
    //connect(api, SIGNAL(infoExtracted(QJsonArray)), this, SLOT(sendResult(QJsonArray)));
    connect(api, SIGNAL(singleInfo(QJsonObject)), this, SLOT(sendSingleResult(QJsonObject)));
    connect(api, SIGNAL(infoExtracted(QJsonArray)), thread, SLOT(quit()));
    thread->start();
}

void InterfaceForQml::sendResult(QJsonArray data)
{
    emit ytData(data);
}

void InterfaceForQml::sendSingleResult(QJsonObject data)
{
    emit singleData(data);
}
