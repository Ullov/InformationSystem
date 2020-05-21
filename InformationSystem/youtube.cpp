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
    cc->setOptions();
    QByteArray data = cc->request(params.value("url").toString() + "/videos");
    KTools::HtmlAst::Object htmlObj = KTools::HtmlAst::Object();
    htmlObj.makeAst(data);
    QString channelTitle = htmlObj.arrsAndObjs.objects[3].value("header").toObject().value("c4TabbedHeaderRender").toObject().value("title").toString();
    QString xsrfToken = QUrl::toPercentEncoding(htmlObj.arrsAndObjs.objects[0].value("XSRF_TOKEN").toString());;
    commentsChunk["X-YouTube-Client-Version"] = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_VERSION").toString();
    commentsChunk["X-YouTube-Client-Name"] = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_NAME").toVariant().toString();
    QJsonArray videosInfo = htmlObj.arrsAndObjs.objects[3].value("contents").toObject().value("twoColumnBrowseResultsRenderer").toObject().value("tabs").toArray()[1].toObject().value("tabRenderer").toObject().value("content").toObject().value("sectionListRenderer").toObject().value("contents").toArray()[0].toObject().value("itemSectionRenderer").toObject().value("contents").toArray()[0].toObject().value("gridRenderer").toObject().value("items").toArray(); // line 45 "items":[...]
    QVector<QString> videosId;
    for (int i = 0; i < videosInfo.size(); i++)
    {
        videosId.append(videosInfo[i].toObject().value("gridVideoRenderer").toObject().value("videoId").toString());
    }

    QVector<QMap<QString, QString>> videosParams;
    for (int i = 0; i < videosId.size(); i++)
    {
        data = cc->request("https://www.youtube.com/watch?v=" + videosId[i]);
        QVector<QVector<QVector<QString>>> regexResult;
        KTools::ExForString::executeRegex(data, {{"([^\"]+___________[^\"]+)\",\"[^\"]+\":\"([^\"]+)"}}, regexResult);
        QMap<QString, QString> tmp;
        tmp["tmp"] = QUrl::toPercentEncoding(regexResult[0][0][1]);
        tmp["tmp2"] = QUrl::toPercentEncoding(regexResult[0][0][2]);
        tmp["data"] = data;
        videosParams.append(tmp);
    }

    QVector<QJsonObject> commentsJsons;
    cc->setRequestType(KTools::Enums::Curl::RequestType::Post);
    cc->currHeaderMode = KTools::Enums::Curl::HeaderMode::Custom;
    cc->currPostParam = "session_token=" + xsrfToken;
    for (int i = 0; i < videosParams.size(); i++)
    {
        cc->setHeader(commentsChunk);
        cc->setOptions();
        data = cc->request("https://www.youtube.com/comment_service_ajax?action_get_comments=1&pbj=1&ctoken=" + videosParams[i]["tmp"]  + "&continuation=" + videosParams[i]["tmp"] + "&itct=" + videosParams[i]["tmp2"]);
        commentsJsons.append(KTools::Converter::convert<QString, QJsonObject>(data));
    }

    for (int i = 0; i < videosParams.size(); i++)
    {
        QJsonArray midResult;
        QJsonArray commsArr = commentsJsons[i].value("response").toObject().value("continuationContents").toObject().value("itemSectionContinuation").toObject().value("contents").toArray();
        for (int j = 0; j < commsArr.size(); j++)
        {
            QJsonObject tmp;
            QJsonObject item = commsArr[j].toObject().value("commentThreadRenderer").toObject().value("comment").toObject().value("commentRenderer").toObject();
            tmp["likes"] = item.value("likeCount");
            tmp["replies"] = item.value("replyCount");
            QJsonArray textTmp = item.value("contentText").toObject().value("runs").toArray();
            QString fullText = "";
            for (int k = 0; k < textTmp.size(); k++)
            {
                fullText += textTmp[k].toObject().value("text").toString();
            }
            tmp["text"] = fullText;
            tmp["opName"] = item.value("authorText").toObject().value("simpleText");
            tmp["opId"] = item.value("authorEndpoint").toObject().value("browseEndpoint").toObject().value("browseId");
            midResult.append(tmp);
        }
        result.append(midResult);
        QJsonObject singleResObj;
        singleResObj["comments"] = midResult;


        KTools::File::writeFile(videosParams[i]["data"].toUtf8(), dir, "videoHtml.txt");
        QVector<QVector<QVector<QString>>> regexResult;
        QVector<QString> patterns = {
            {R"raw(\\"title\\":\\"([^"]+)")raw"}, // title
            {R"raw(,\\"lengthSeconds\\":\\"([^"]+)",)raw"}, // lenght
            {R"raw(,\\"keywords\\":(\[[^]]+\]))raw"}, // keywords
            {R"raw("tooltip":"([^ ]+) \/ ([^"]+)"}})raw"} // likes and dislikes
        };
        KTools::ExForString::executeRegex(videosParams[i]["data"], patterns, regexResult);
        singleResObj["title"] = regexResult[0][0][1].replace("\\", "");
        singleResObj["length"] = regexResult[1][0][1].replace("\\", "");
        singleResObj["keywords"] = KTools::Converter::convert<QString, QJsonArray>(regexResult[2][0][1].replace("\\", ""));
        singleResObj["likes"] = regexResult[3][0][1];
        singleResObj["dislikes"] = regexResult[3][0][2];
        emit singleInfo(singleResObj);

        QString nope;
    }

    emit infoExtracted(result);
}
// \\"title\\":\\"([^"]+)",\\"lengthSeconds\\":\\"([^"]+)",\\"keywords\\":(\[[^]]+\])
// "dislike this video along with ([\d,]+) other people"
// "like this video along with ([\d,]+) other people"
// "likeStatus":"LIKE","tooltip":"([^ ]+) \/ ([^"]+)"
// "likeStatus":"LIKE","tooltip":"([^"]+)"
// "tooltip":"([^ ]+) \/ ([^"]+)"}},"superTitleLink"
// "tooltip":"([^ ]+) \/ ([^"]+)"}}
// \\"title\\":\\"([^"]+)"
// ,\\"lengthSeconds\\":\\"([^"]+)",
