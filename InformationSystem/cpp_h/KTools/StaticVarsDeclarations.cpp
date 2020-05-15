#include "options.h"
#include "log.h"
#include "curl.h"
#include "enums.h"
#include "ktoolsqmlinterface.h"
//#include "../Parsers/parsersqmlinterface.h"
#include "fileidentifier.h"

// Start OptionsHandler
QString KTools::Options::configFile = "config.txt";
QString KTools::Options::configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/InformationSystem/";
QString KTools::Options::rootProgramPath = "";
QString KTools::Options::logRootPath = "";

QMap<KTools::Enums::Parsers, QString> KTools::Options::parsersWritePathes = {
    {KTools::Enums::Parsers::ExHentai, rootProgramPath + "/Downloads/Manga/ExHentai/"},
    {KTools::Enums::Parsers::MangaDex, rootProgramPath + "/Downloads/Manga/MangaDex/"},
    {KTools::Enums::Parsers::MangaIro, rootProgramPath + "/Downloads/Manga/MangaIro/"},
    {KTools::Enums::Parsers::NHentaiDotNet, rootProgramPath + "/Downloads/Manga/NHentai.net/"},
    {KTools::Enums::Parsers::Pixiv, rootProgramPath + "/Downloads/Images/Pixiv/"},
    {KTools::Enums::Parsers::Twitter, rootProgramPath + "/Downloads/Mixed/Twitter/"},
    {KTools::Enums::Parsers::VK, rootProgramPath + "/Downloads/Mixed/VK/"},
    {KTools::Enums::Parsers::YouTube, rootProgramPath + "/Downloads/Video/YouTube/"},
    {KTools::Enums::Parsers::NineHentai, rootProgramPath + "/Downloads/Manga/9Hentai/"},
    {KTools::Enums::Parsers::MangaKakalot, rootProgramPath + "/Downloads/Manga/MangaKakalot/"},
    {KTools::Enums::Parsers::NHentaiDotCom, rootProgramPath + "/Downloads/Manga/Nhentai.com/"}
};
QMap<KTools::Enums::Parsers, QString> KTools::Options::parsersNames = {
    {KTools::Enums::Parsers::ExHentai, "ExHentai"},
    {KTools::Enums::Parsers::MangaDex, "MangaDex"},
    {KTools::Enums::Parsers::MangaIro, "MangaIro"},
    {KTools::Enums::Parsers::NHentaiDotNet, "NHentai.net"},
    {KTools::Enums::Parsers::Pixiv, "Pixiv"},
    {KTools::Enums::Parsers::Twitter, "Twitter"},
    {KTools::Enums::Parsers::VK, "VK"},
    {KTools::Enums::Parsers::YouTube, "YouTube"},
    {KTools::Enums::Parsers::NineHentai, "9Hentai"},
    {KTools::Enums::Parsers::MangaKakalot, "MangaKakalot"},
    {KTools::Enums::Parsers::NHentaiDotCom, "NHentai.com"}
};
QJsonObject KTools::Options::configsObj = QJsonObject();
// End OptionsHandler
// Start Log
QString KTools::Log::logFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd") + ".log";

QMap<KTools::Enums::LogType, QString> KTools::Log::logTypePath = {
    {KTools::Enums::LogType::Info, "/info/"},
    {KTools::Enums::LogType::Debug, "/debug/"},
    {KTools::Enums::LogType::Error, "/error/"},
    {KTools::Enums::LogType::Custom, "/custom/"}
};
QMap<KTools::Enums::LogType, QString> KTools::Log::logTypeNames = {
    {KTools::Enums::LogType::Info, "Info"},
    {KTools::Enums::LogType::Debug, "Debug"},
    {KTools::Enums::LogType::Error, "Error"},
    {KTools::Enums::LogType::Custom, "Custom"}
};
// End Log
// Start CurlClass
QString KTools::Curl::cacertPath = QDir::temp().path() + "/InfSyst";
QString KTools::Curl::cacertFileName = "cacert.pem";
QString KTools::Curl::fullCacertPath = cacertPath + '/' + cacertFileName;
QString KTools::Curl::pathToCacertInQrc = ":/resources/other/cacert.pem";
QString KTools::Curl::cookiePath = "";
// End CurlClass
// Start KTolsQmlInterface
KToolsQmlInterface* KToolsQmlInterface::obj = new KToolsQmlInterface();
// End KTolsQmlInterface
// Start ParsersQmlInterface
//ParsersQmlInterface ParsersQmlInterface::obj = ParsersQmlInterface();
// End ParsersQmlInterface
