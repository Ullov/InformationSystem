#include "interfaceforqml.h"

InterfaceForQml::InterfaceForQml() {}

void InterfaceForQml::getData(const QJsonObject &params)
{
    QtConcurrent::run([=](){
        YouTube *api = new YouTube();
        api->params = params;
        api->extractInfo();
        delete api;
    });
}

void InterfaceForQml::sendResult(const QJsonObject &data)
{

}
