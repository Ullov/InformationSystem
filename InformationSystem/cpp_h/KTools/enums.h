#ifndef KTOOLS_ENUMS_H
#define KTOOLS_ENUMS_H

#include <QMap>
#include <QObject>
#include <QQmlEngine>

namespace KTools::Enums
{
    enum class LogType {
        Custom,
        Info,
        Error,
        Debug
    };
    namespace Curl {
        enum class RequestType {
            Get,
            Post
        };
        enum class CookieMode {
            None,
            GetAllTimes,
            GetAndKeep,
            Keep,
            Void
        };
        enum class HeaderMode {
            None,
            Custom
        };
        enum class SetHeaderMode {
            Add,
            New
        };
        enum class HttpVersion {
            Http2,
            Standart
        };
        enum class RequestMode {
            Old,
            New
        };
    }
};

#endif // KTOOLS_ENUMS_H
