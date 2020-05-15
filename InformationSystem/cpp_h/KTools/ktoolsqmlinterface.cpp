#include "ktoolsqmlinterface.h"

KToolsQmlInterface::KToolsQmlInterface() {}

void KToolsQmlInterface::optionsHandlerSetRootProgramPath(const QString path)
{
    KTools::Options::setRootProgramPath(path);
}

void KToolsQmlInterface::optionsHandlerEmitRootProgramPath()
{
    emit optionsHandlerSendRootProgramPath(KTools::Options::rootProgramPath);
}

void KToolsQmlInterface::optionsHandlerSlotGetParam(const QString pathToParam)
{
    emit optionsHandlerSignalParam(pathToParam, KTools::Options::getParam(pathToParam));
}

void KToolsQmlInterface::optionsHandlerSlotSetParam(const QString pathToParam, const QString param)
{
    KTools::Options::setParam(pathToParam, param);
}

void KToolsQmlInterface::curlSendProgress(const QList<double> list, const qint64 millisecondsFromStart, const KTools::Enums::Parsers downloaderType)
{
    emit obj->curlProgressSignal(list, millisecondsFromStart, downloaderType);
}
