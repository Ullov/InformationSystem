#include "youtube.h"

YouTube::YouTube()
{
    //netLoop = new QEventLoop();
    netManager = new QNetworkAccessManager();
}

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
        //{"X-YouTube-Client-Name", "1"},
        //{"X-YouTube-Client-Version", "2.20200429.03.00"}
        //{"X-YouTube-Utc-Offset", "180"},
        //{"X-YouTube-Time-Zone", "Europe/Moscow"},
        //{"X-YouTube-Ad-Signals", "dt=1588718969166&flash=0&frm&u_tz=180&u_his=7&u_java&u_h=1050&u_w=1680&u_ah=1010&u_aw=1680&u_cd=24&u_nplug&u_nmime&bc=29&bih=319&biw=1663&brdim=-8%2C-8%2C-8%2C-8%2C1680%2C0%2C1696%2C1026%2C1680%2C319&vis=1&wgl=true&ca_type=image"}
        //{"Accept-Language", "ja,en-US;q=0.7,en;q=0.3"}
    };
    QString dir = KTools::Options::configPath + "/tmp";
    cc->setErrFile(dir, "errfile.txt");
    cc->currHeaderMode = KTools::Enums::Curl::HeaderMode::None;
    cc->currCookieMode = KTools::Enums::Curl::CookieMode::GetAllTimes;
    cc->cookieFileName = "cookieYoutube.txt";
    cc->cookiePath = KTools::Options::configPath;
    //cc->setHeader({{"Accept", "*/*"},{"Host", "www.youtube.com"},});
    cc->setOptions();
    QByteArray data = cc->request(params.value("url").toString() + "/videos");
    KTools::HtmlAst::Object htmlObj = KTools::HtmlAst::Object();
    htmlObj.makeAst(data);
    QString xsrfToken = QUrl::toPercentEncoding(htmlObj.arrsAndObjs.objects[0].value("XSRF_TOKEN").toString());;
    commentsChunk["X-YouTube-Client-Version"] = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_VERSION").toString();
    commentsChunk["X-YouTube-Client-Name"] = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_NAME").toVariant().toString();
    QJsonArray videosInfo = htmlObj.arrsAndObjs.objects[3].value("contents").toObject().value("twoColumnBrowseResultsRenderer").toObject().value("tabs").toArray()[1].toObject().value("tabRenderer").toObject().value("content").toObject().value("sectionListRenderer").toObject().value("contents").toArray()[0].toObject().value("itemSectionRenderer").toObject().value("contents").toArray()[0].toObject().value("gridRenderer").toObject().value("items").toArray(); // line 45 "items":[...]
    QVector<QString> videosId;
    for (int i = 0; i < videosInfo.size(); i++)
    {
        videosId.append(videosInfo[i].toObject().value("gridVideoRenderer").toObject().value("videoId").toString());
    }

    data = cc->request("https://www.youtube.com/watch?v=" + videosId[1]);
    htmlObj = KTools::HtmlAst::Object();
    htmlObj.makeAst(data);
    //QString xsrfToken = htmlObj.arrsAndObjs.objects[0].value("XSRF_TOKEN").toString(); // aka session token
    //QString clientName = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_NAME").toString();
    //QString clientVersion = htmlObj.arrsAndObjs.objects[0].value("INNERTUBE_CONTEXT_CLIENT_VERSION").toString();
    QJsonArray continuations = htmlObj.arrsAndObjs.objects[5].value("contents").toObject().value("twoColumnWatchNextResults").toObject().value("secondaryResults").toObject().value("secondaryResults").toObject().value("continuations").toArray();
    QVector<QJsonObject> commentsJsons;
    cc->restartSession();
    cc->setRequestType(KTools::Enums::Curl::RequestType::Post);
    cc->currHeaderMode = KTools::Enums::Curl::HeaderMode::Custom;

    /*commentsChunk["X-YouTube-Variants-Checksum"] = htmlObj.arrsAndObjs.objects[0].value("VARIANTS_CHECKSUM").toString();
    commentsChunk["X-YouTube-Page-CL"] = htmlObj.arrsAndObjs.objects[0].value("PAGE_CL").toVariant().toString();
    commentsChunk["X-YouTube-Device"] = htmlObj.arrsAndObjs.objects[0].value("DEVICE").toString();
    commentsChunk["X-YouTube-Page-Label"] = htmlObj.arrsAndObjs.objects[0].value("PAGE_BUILD_LABEL").toString();
    commentsChunk["X-SPF-Referer"] = "https://www.youtube.com/watch?v=" + videosId[1];
    commentsChunk["X-SPF-Previous"] = "https://www.youtube.com/watch?v=" + videosId[1];
    commentsChunk["Referer"] = "https://www.youtube.com/watch?v=" + videosId[1];*/
    //commentsChunk["X-Youtube-Identity-Token"] = htmlObj.arrsAndObjs.objects[0].value("EVENT_ID").toString();
    //commentsChunk["Cookie"] = "YSC=yoAjlJ6hDqY; VISITOR_INFO1_LIVE=W-ljvCI5kis; GPS=1";

    //KTools::Curl cc2 = KTools::Curl();

    QVector<QVector<QVector<QString>>> regexResult;
    //QString pattern = "\"([^\"]+___________[^\"]+)\"";
    KTools::ExForString::executeRegex(data, {{"([^\"]+___________[^\"]+)\",\"[^\"]+\":\"([^\"]+)"}}, regexResult);


    cc->setHeader(commentsChunk);
    cc->setOptions();

    QString tmp = QUrl::toPercentEncoding(regexResult[0][0][1]);
    QString tmp2 = QUrl::toPercentEncoding(regexResult[0][0][2]);
    cc->currPostParam = "session_token=" + xsrfToken;
    /*cc->currPostParam = "session_token=" + QUrl::toPercentEncoding(htmlObj.arrsAndObjs.objects[0].value("XSRF_TOKEN").toString());*/
    //commentsJsons.append(KTools::Converter::convert<QString, QJsonObject>(cc->request("https://www.youtube.com/comment_service_ajax?action_get_comments=1&pbj=1&ctoken=" + tmp + "&continuation=" + tmp + "&itct=" + tmp2)));
    data = cc->request("https://www.youtube.com/comment_service_ajax?action_get_comments=1&pbj=1&ctoken=" + tmp + "&continuation=" + tmp + "&itct=" + tmp2);

    KTools::File::writeFile((tmp + "\n" + tmp2 + "\n" + cc->currPostParam).toUtf8(), dir, "requestParams.txt");

    //cc->currPostParam = "video_id=" + videosId[1] + "&session_token=" + QUrl::toPercentEncoding(htmlObj.arrsAndObjs.objects[0].value("XSRF_TOKEN").toString());
    //data = cc->request("https://www.youtube.com/comment_ajax?action_load_comments=1&order_by_time=true&filter=" + videosId[1]);

    //QJsonObject commentsJson = KTools::Converter::convert<QString, QJsonObject>(cc->request("https://www.youtube.com/comment_service_ajax"));
    //KTools::File::writeFile(KTools::Converter::convert<QJsonObject, QByteArray>(htmlObj.arrsAndObjs.objects[3]), dir, "infoVideos.json");
    cc->unsetErrFile();
    QString nope;
}

void YouTube::get(const QString &url)
{
    QNetworkReply *reply;
    QEventLoop netLoop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &netLoop, SLOT(quit()));
    reply = netManager->get(QNetworkRequest(QUrl(url)));
    netLoop.exec();
    netData = reply->readAll();
    reply->deleteLater();
}

void YouTube::post(const QString &url)
{
    QNetworkReply *reply;
    QEventLoop netLoop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &netLoop, SLOT(quit()));
    reply = netManager->post(QNetworkRequest(QUrl(url)), postData);
    netLoop.exec();
    netData = reply->readAll();
    reply->deleteLater();
}
