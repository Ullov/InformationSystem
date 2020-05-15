#ifndef KTOOLS_ENUMS_H
#define KTOOLS_ENUMS_H

#include <QMap>
#include <QObject>
#include <QQmlEngine>

namespace KTools::Enums
{
    Q_NAMESPACE
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    enum class Parsers {
        ExHentai,
        MangaDex,
        MangaIro,
        NHentaiDotNet,
        Pixiv,
        Twitter,
        VK,
        YouTube,
        NineHentai,
        MangaKakalot,
        NHentaiDotCom
    };
    enum class LogType {
        Custom,
        Info,
        Error,
        Debug
    };
    Q_ENUM_NS(Parsers)
    Q_ENUM_NS(LogType)
    namespace ParserModes
    {
        Q_NAMESPACE
        Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
        enum class ExHentai {
            Download,
            FrontPage
        };
        enum class MangaDex {
            Download
        };
        enum class MangaIro {
            Download
        };
        enum class NHentaiDotNet {
            Download
        };
        enum class Pixiv {
            Download
        };
        enum class Twitter {
            Download
        };
        enum class VK {
            Download
        };
        enum class YouTube {
            Download
        };
        enum class NineHentai {
            Download
        };
        enum class MangaKakalot {
            Download
        };
        enum class NHentaiDotCom {
            Download
        };

        Q_ENUM_NS(ExHentai)
        Q_ENUM_NS(MangaDex)
        Q_ENUM_NS(MangaIro)
        Q_ENUM_NS(NHentaiDotNet)
        Q_ENUM_NS(Pixiv)
        Q_ENUM_NS(Twitter)
        Q_ENUM_NS(VK)
        Q_ENUM_NS(YouTube)
        Q_ENUM_NS(NineHentai)
        Q_ENUM_NS(MangaKakalot)
        Q_ENUM_NS(NHentaiDotCom)
        static void registerTypesForQml()
        {
            qmlRegisterUncreatableMetaObject(ParserModes::staticMetaObject, "KEnums", 1, 0, "ParserModes", "Is enum");
            qRegisterMetaType<ExHentai>("ParserModesExHentai");
            qRegisterMetaType<MangaDex>("ParserModesMangaDex");
            qRegisterMetaType<MangaIro>("ParserModesMangaIro");
            qRegisterMetaType<NHentaiDotNet>("ParserModesNHentaiDotNet");
            qRegisterMetaType<Pixiv>("ParserModesPixiv");
            qRegisterMetaType<Twitter>("ParserModesTwitter");
            qRegisterMetaType<VK>("ParserModesVK");
            qRegisterMetaType<YouTube>("ParserModesYouTube");
            qRegisterMetaType<NineHentai>("ParserModesNineHentai");
            qRegisterMetaType<MangaKakalot>("ParserModesMangaKakalot");
            qRegisterMetaType<NHentaiDotCom>("ParserModesNHentaiDotCom");
        }
    };
    namespace Curl {
        Q_NAMESPACE
        Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
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

        Q_ENUM_NS(SetHeaderMode)
        Q_ENUM_NS(RequestType)
        Q_ENUM_NS(CookieMode)
        Q_ENUM_NS(HeaderMode)
        inline void registerTypesForQml()
        {
            qmlRegisterUncreatableMetaObject(staticMetaObject, "KEnums", 1, 0, "CurlSettings", "Is enum");
            qRegisterMetaType<RequestType>("CurlSettingsRequestType");
            qRegisterMetaType<CookieMode>("CurlSettingsCookieMode");
            qRegisterMetaType<HeaderMode>("CurlSettingsHeaderMode");
            qRegisterMetaType<SetHeaderMode>("CurlSettingsSetHeaderMode");
        }
    }
    inline void registerTypesForQml()
    {
        qmlRegisterUncreatableMetaObject(staticMetaObject, "KEnums", 1, 0, "KEnums", "Is enum");
        qRegisterMetaType<Parsers>("KEnumParsers");
        qRegisterMetaType<LogType>("KEnumLogType");
        Curl::registerTypesForQml();
        ParserModes::registerTypesForQml();
    }
};

#endif // KTOOLS_ENUMS_H
