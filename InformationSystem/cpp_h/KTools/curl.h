#ifndef KTOOLS_CURL_H
#define KTOOLS_CURL_H

#include <curl/curl.h>
#include <string>
#include <vector>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QElapsedTimer>
#include "file.h"
#include "enums.h"
#include "log.h"
#include "ktoolsqmlinterface.h"

namespace KTools
{
    class Curl
    {
    public:
        Curl();
        ~Curl();

        QByteArray performing(const char* url);
        void setHeader(QVector<QByteArray> chunk, const KTools::Enums::Curl::SetHeaderMode mode = KTools::Enums::Curl::SetHeaderMode::Add);
        QByteArray request(const QString &url);
        void restartSession();


        void setOptions();
        void setRequestType(KTools::Enums::Curl::RequestType requType);
        void setHeader(const QMap<QString, QString> &chunk);
        void setHeaderLine(const std::pair<QString, QString> &headerLine);
        void delHeaderLine(const QString &key);
        void setErrFile(const QString &path, const QString &fileName);
        void unsetErrFile();
        void setCookie();

        void addHandle(const QString &handleName);
        void swichHandle (const QString &handleName);

        KTools::Enums::Curl::CookieMode currCookieMode;
        KTools::Enums::Curl::HeaderMode currHeaderMode;
        KTools::Enums::Curl::RequestType currRequestType;
        KTools::Enums::Curl::HttpVersion currHttpVersion;
        KTools::Enums::Curl::RequestMode currRequestMode;
        QString currPostParam;
        static QString cookiePath;
        QString cookieFileName;
        bool emitProgress = false;
        long responseCode; // response code for last request
        bool autoRefer = true;
        bool saveHeader = false;
        QByteArray savedHeaderData;
        FILE *stdErr;
        bool errFileSetted = false;
        bool verboseOutput = false;

    private:
        CURLcode res;
        static quint64 writeMemoryCallback(char *data, quint64 size, quint64 nmemb, QByteArray *writerData);
        static int XFerInfoFunctionCallback(void *p, double dlTotal, double dlNow, double ulTotal, double ulNow);
        static quint64 headerCallback(char *buffer, quint64 size, quint64 nitems, QByteArray *userdata);
        void generateHeader();

        struct curl_slist *header;
        struct curl_slist *cookies;
        static QString cacertPath;
        static QString cacertFileName;
        static QString fullCacertPath;
        static QString pathToCacertInQrc;
        CURL *gCurlHandle;
        QMap<QString, QString> headerData;
        QMap<QString, CURL*> handlesList;

        struct ForProggress
        {
            Curl *th;
            QElapsedTimer timer;
            double lastDlTotal;
            double lastUlTotal;
            double lastDlNow;
            double lastUlNow;
        };
    };
}

#endif // KTOOLS_CURL_H
