#include "log.h"

KTools::Log::Log() {}

void KTools::Log::writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:zzz");
    QString dt = '[' + from + ']' + dateTime + ' ' + message + '\n';
    File::writeFile(dt.toUtf8(), Options::logRootPath + logTypePath[type], logFileName, QIODevice::Append | QIODevice::WriteOnly);
}

void KTools::Log::writeCustomLog(const QString &message, const QString &from, const KTools::Enums::LogType &type, const QString &path, const QString &fileName)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:zzz");
    QString dt = '[' + logTypeNames[type] + "][" + from + ']' + dateTime + ' ' + message + '\n';
    File::writeFile(dt.toUtf8(), path, fileName, QIODevice::Append | QIODevice::WriteOnly);
}

void KTools::Log::writeInfo(const QString &message, const QString &from)
{
    writeCustomLog(message, from, KTools::Enums::LogType::Info);
}

void KTools::Log::writeError(const QString &message, const QString &from)
{
    writeCustomLog(message, from, KTools::Enums::LogType::Error);
}

void KTools::Log::writeDebug(const QString &message, const QString &from)
{
    writeCustomLog(message, from, KTools::Enums::LogType::Debug);
}
