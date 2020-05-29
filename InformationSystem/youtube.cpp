#include "youtube.h"

YouTube::YouTube() {}

void YouTube::extractInfo()
{
    QMap<QString, QString> commentsChunk = {
        {"Accept", "*/*"},
        {"Host", "www.youtube.com"},
        {"Content-Type", "application/x-www-form-urlencoded"},
        {"Origin", "https://www.youtube.com"},
        {"DNT", "1"},
        {"Pragma", "no-cache"},
        {"Cache-control", "no-cache"},
        {"TE", "Trailers"},
    };
    QString dir = KTools::Options::configPath + "/tmp";
    cc->currHeaderMode = KTools::Enums::Curl::HeaderMode::None;
    cc->currCookieMode = KTools::Enums::Curl::CookieMode::GetAllTimes;
    cc->cookieFileName = "cookieYoutube.txt";
    cc->cookiePath = KTools::Options::configPath;
    cc->setOptions();
    QByteArray data = cc->request("https://www.youtube.com");
    KTools::HtmlAst::Object htmlObj = KTools::HtmlAst::Object();
    htmlObj.makeAst(data);
    //QString xsrfToken = QUrl::toPercentEncoding(htmlObj.arrsAndObjs.objects[0].value("XSRF_TOKEN").toString());;
    commentsChunk["X-YouTube-Client-Version"] = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_VERSION").toString();
    commentsChunk["X-YouTube-Client-Name"] = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_NAME").toVariant().toString();
    cc->currHeaderMode = KTools::Enums::Curl::HeaderMode::Custom;
    cc->setHeader(commentsChunk);
    cc->setOptions();
    QJsonArray jsonArr = KTools::Converter::convert<QString, QJsonArray>(cc->request("https://www.youtube.com/results?search_query=" + QUrl::toPercentEncoding(params.value("url").toString()) + "&sp=EgIQAQ%253D%253D&pbj=1"));
    QJsonArray videosInfo = jsonArr[1].toObject().value("response").toObject().value("contents").toObject().value("twoColumnSearchResultsRenderer").toObject().value("primaryContents").toObject().value("sectionListRenderer").toObject().value("contents").toArray()[0].toObject().value("itemSectionRenderer").toObject().value("contents").toArray();
    QVector<QString> videosId;
    for (int i = 0; i < videosInfo.size(); i++)
    {
        if (!videosInfo[i].toObject().value("videoRenderer").isUndefined())
            videosId.append(videosInfo[i].toObject().value("videoRenderer").toObject().value("videoId").toString());
    }

    for (int i = 0; i < videosId.size(); i++)
    {
        data = cc->request("https://www.youtube.com/watch?v=" + videosId[i]);
        QVector<QVector<QVector<QString>>> regexResult;
        QString videoHtmlPage = data;
        QJsonObject singleResObj;
        regexResult = QVector<QVector<QVector<QString>>>();
        QVector<QString> patterns = {
            {R"raw(,\\"keywords\\":(\[[^]]+\]))raw"}, // keywords
        };
        KTools::ExForString::executeRegex(videoHtmlPage, patterns, regexResult);
        if (regexResult[0].size() > 0)
            singleResObj["keywords"] = KTools::Converter::convert<QString, QJsonArray>(regexResult[0][0][1].replace("\\", ""));

        emit singleInfo(singleResObj);

        QString nope;
    }

    emit infoExtracted(result);
}
