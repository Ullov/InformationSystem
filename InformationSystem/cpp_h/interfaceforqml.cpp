#include "interfaceforqml.h"

#include <QFutureWatcher>

InterfaceForQml::InterfaceForQml() {}

void InterfaceForQml::getData(const QJsonObject &params)
{
    //QFutureWatcher<QJsonArray> futWat;
    //new (&futWat) (QFutureWatcher<QJsonArray>);
    //connect(&futWat, SIGNAL(finished()), this, SLOT(sendResult()));
    //QJsonArray *res;
//    par = new QJsonObject(params);
//    QFuture<void> fut = QtConcurrent::run([this](){
//        // *api = new YouTube();
//        api->params = *this->par;
//        api->extractInfo();
//        //res = new QJsonArray(api->result);
//        this->sendResult();
//        delete api;
//        //return res;
//    });
    //futWat.setFuture(fut);
    QThread *thread = new QThread();
    api = new YouTube();
    api->params = params;
    api->moveToThread(thread);
    connect(thread, SIGNAL(started()), api, SLOT(extractInfo()));
    connect(api, SIGNAL(infoExtracted(QJsonArray)), this, SLOT(sendResult(QJsonArray)));
    connect(api, SIGNAL(infoExtracted(QJsonArray)), thread, SLOT(quit()));
    thread->start();
}

void InterfaceForQml::sendResult(QJsonArray data)
{
    emit ytData(data);
}
