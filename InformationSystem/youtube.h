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
    QJsonArray result;

public slots:
    void extractInfo();

signals:
    void infoExtracted(QJsonArray info);
    void singleInfo(QJsonObject info);
};

#endif // YOUTUBE_H
