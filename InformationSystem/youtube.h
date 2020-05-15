#ifndef YOUTUBE_H
#define YOUTUBE_H

#include "cpp_h/parserclass.h"
#include <QTimeZone>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class YouTube : public ParserClass
{
    Q_OBJECT
public:
    YouTube();

    QJsonObject params;
    QJsonObject result;

    void extractInfo();

private:
    //QEventLoop *netLoop;
    QNetworkAccessManager *netManager;

    void get(const QString &url);

signals:
    void infoExtracted(const QJsonObject &info);
};

#endif // YOUTUBE_H