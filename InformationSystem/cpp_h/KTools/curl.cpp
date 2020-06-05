#include "curl.h"


KTools::Curl::Curl()
{
    if (!KTools::File::fileExist(fullCacertPath))
        KTools::File::copyFile(pathToCacertInQrc, cacertPath, cacertFileName);
    //OptionsHandler::rootProgramPath + "/Cookie/";
    gCurlHandle = curl_easy_init();
    handlesList["main"] = gCurlHandle;
    currCookieMode = KTools::Enums::Curl::CookieMode::Void;
    currHeaderMode = KTools::Enums::Curl::HeaderMode::Custom;
    currRequestType = KTools::Enums::Curl::RequestType::Get;
    currHttpVersion = KTools::Enums::Curl::HttpVersion::Http2;
    currRequestMode = KTools::Enums::Curl::RequestMode::New;
}

KTools::Curl::~Curl()
{
    if (errFileSetted)
        unsetErrFile();

    QMap<QString, CURL*>::iterator end = handlesList.end();
    for (QMap<QString, CURL*>::iterator i = handlesList.begin(); i != end; ++i)
    {
        curl_easy_cleanup(i.value());
    }
    if (cookies)
    {
        //curl_slist_free_all(cookies);
    }
}

QByteArray KTools::Curl::performing(const char* url)
{
    ForProggress fp;

    QByteArray buffer = "";
    CURL *curlHandle = curl_easy_init(); // init curl session

    curl_easy_setopt(curlHandle, CURLOPT_URL, url); // specifu url to get
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback); // send all data to this function
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curlHandle, CURLOPT_CAINFO,  fullCacertPath.toStdString().c_str());
    curl_easy_setopt(curlHandle, CURLOPT_PROXY_SSL_VERIFYHOST, 1);
    curl_easy_setopt(curlHandle, CURLOPT_PROXY_SSL_VERIFYPEER, 1);
    curl_easy_setopt(curlHandle, CURLOPT_ACCEPT_ENCODING, "deflate, gzip");
    curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curlHandle, CURLOPT_PROGRESSFUNCTION, XFerInfoFunctionCallback);
    curl_easy_setopt(curlHandle, CURLOPT_PROGRESSDATA, &fp);
    curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 0);

    fp.th = this;
    fp.timer = QElapsedTimer();
    fp.timer.start();
    res = curl_easy_perform(curlHandle);

    curl_easy_cleanup(curlHandle);
    //curl_global_cleanup();

    if (res != CURLE_OK)
    {
        return "Error in CURL";
    }
    else
    {
        return buffer;
    }
}

quint64 KTools::Curl::writeMemoryCallback(char *data, quint64 size, quint64 nmemb, QByteArray *writerData)
{
    if (writerData == NULL)
        return 0;
    writerData->append(data, size*nmemb);
    return size * nmemb;
}

void KTools::Curl::setHeader(QVector<QByteArray> chunk, const KTools::Enums::Curl::SetHeaderMode mode)
{
    header = NULL;
    for (qint64 i = 0; i < chunk.size(); i++)
    {
        header = curl_slist_append(header, chunk[i]);
    }
}

int KTools::Curl::XFerInfoFunctionCallback(void *p, double dlTotal, double dlNow, double ulTotal, double ulNow)
{ // all here in bytes
    ForProggress *fp = static_cast<ForProggress*>(p);
    if (fp->lastDlNow != dlNow || fp->lastDlTotal != dlTotal || fp->lastUlTotal != ulTotal || fp->lastUlNow != ulNow)
    {
        QList<double> list;
        list.append(dlNow);
        list.append(dlTotal);
        list.append(ulNow);
        list.append(ulTotal);
        //KToolsQmlInterface::curlSendProgress(list, fp->timer.elapsed(), fp->th->downloaderType);
        fp->lastDlNow = dlNow;
        fp->lastDlTotal = dlTotal;
        fp->lastUlTotal = ulTotal;
        fp->lastUlNow = ulNow;
    }
    return CURLE_OK;
}

QByteArray KTools::Curl::request(const QString &url)
{
    if (currRequestMode == KTools::Enums::Curl::RequestMode::Old)
    {
        return performing(url.toStdString().c_str());
    }
    QByteArray buffer;
    std::string c = currPostParam.toStdString();

    curl_easy_setopt(gCurlHandle, CURLOPT_URL, url.toStdString().c_str()); // specify url to get
    curl_easy_setopt(gCurlHandle, CURLOPT_WRITEDATA, &buffer);

    if (currRequestType == KTools::Enums::Curl::RequestType::Post)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_POSTFIELDS, c.c_str());
        curl_easy_setopt(gCurlHandle, CURLOPT_POSTFIELDSIZE, currPostParam.size());
    }

    if (currHeaderMode == KTools::Enums::Curl::HeaderMode::Custom)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_HTTPHEADER, header);
    }

    savedHeaderData.clear();

    res = curl_easy_perform(gCurlHandle);
    curl_easy_getinfo(gCurlHandle, CURLINFO_RESPONSE_CODE, &responseCode);
    curl_easy_getinfo(gCurlHandle, CURLINFO_COOKIELIST, &cookies);

    if (res != CURLE_OK)
    {
        KTools::Log::writeError("Error in request. URL: " + url, "CurlClass");
        return "Error in CURL";
    }
    else
    {
        return buffer;
    }
}

void KTools::Curl::restartSession()
{
    if (gCurlHandle)
        curl_easy_cleanup(gCurlHandle);

    gCurlHandle = curl_easy_init();
}

quint64 KTools::Curl::headerCallback(char *buffer, quint64 size, quint64 nitems, QByteArray *userdata)
{
    userdata->append(buffer, nitems * size);
    return nitems * size;
}

void KTools::Curl::setOptions()
{
    curl_easy_setopt(gCurlHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback); // send all data to this function
    curl_easy_setopt(gCurlHandle, CURLOPT_CAINFO,  fullCacertPath.toStdString().c_str());
    curl_easy_setopt(gCurlHandle, CURLOPT_PROXY_SSL_VERIFYHOST, 1);
    curl_easy_setopt(gCurlHandle, CURLOPT_PROXY_SSL_VERIFYPEER, 1);
    curl_easy_setopt(gCurlHandle, CURLOPT_ACCEPT_ENCODING, "gzip, deflate, br");
    curl_easy_setopt(gCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(gCurlHandle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(gCurlHandle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");

    if (currHttpVersion == KTools::Enums::Curl::HttpVersion::Http2)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
    }

    /*
     * This if else construction choose which request type do
     */
    setRequestType(currRequestType);

    /*
     * This if else construction choose what to do with cookies
     */
    if (currCookieMode == KTools::Enums::Curl::CookieMode::GetAndKeep) // Get cookie in first request and use it in next requests
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_COOKIEJAR, (cookiePath + cookieFileName).toStdString().c_str());
        currCookieMode = KTools::Enums::Curl::CookieMode::Keep;
    }
    else if (currCookieMode == KTools::Enums::Curl::CookieMode::Keep) // Use cookie from file
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_COOKIEFILE, (cookiePath + cookieFileName).toStdString().c_str());
    }
    else if (currCookieMode == KTools::Enums::Curl::CookieMode::GetAllTimes) // Get new cookie in each request
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_COOKIEJAR, (cookiePath + cookieFileName).toStdString().c_str());
        curl_easy_setopt(gCurlHandle, CURLOPT_COOKIEFILE, (cookiePath + cookieFileName).toStdString().c_str());
    }
    else if (currCookieMode == KTools::Enums::Curl::CookieMode::Void) // Only starts cookie engine
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_COOKIEFILE, "");
    }


    /*
     * This if construction choose what to do with header
     */
    if (currHeaderMode == KTools::Enums::Curl::HeaderMode::Custom) // Sets header
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_HTTPHEADER, header);
    }

    /*
     * This if construction choose send progress data or not (on default emitProgress = false)
     */
    ForProggress fp;
    if (emitProgress)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_PROGRESSFUNCTION, XFerInfoFunctionCallback);
        curl_easy_setopt(gCurlHandle, CURLOPT_PROGRESSDATA, &fp);
        curl_easy_setopt(gCurlHandle, CURLOPT_NOPROGRESS, 0);
        fp.th = this;
        fp.timer = QElapsedTimer();
        fp.timer.start();
    }

    /*
     * This if construction choose set automatically refer field or not (on default autoRefer = true)
     */
    if (autoRefer)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_AUTOREFERER, 1L);
    }

    /*
     * This if construction choose set automatically refer field or not (on default autoRefer = true)
     */
    //savedHeaderData.clear();
    if (saveHeader)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_HEADERDATA, &savedHeaderData);
        curl_easy_setopt(gCurlHandle, CURLOPT_HEADERFUNCTION, headerCallback);
    }

    if (verboseOutput)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(gCurlHandle, CURLOPT_STDERR, stdErr);
    }
}

void KTools::Curl::setRequestType(KTools::Enums::Curl::RequestType requType)
{
    currRequestType = requType;
    if (requType == KTools::Enums::Curl::RequestType::Get)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_CUSTOMREQUEST, "GET");
        //curl_easy_setopt(gCurlHandle, CURLOPT_HTTPGET, 1L);
    }
    else if (requType == KTools::Enums::Curl::RequestType::Post)
    {
        curl_easy_setopt(gCurlHandle, CURLOPT_CUSTOMREQUEST, "POST");
        //curl_easy_setopt(gCurlHandle, CURLOPT_POST, 1L);
    }
}

void KTools::Curl::generateHeader()
{
    header = NULL;
    QMap<QString, QString>::iterator end = headerData.end();
    for (QMap<QString, QString>::iterator i = headerData.begin(); i != end; ++i)
    {
        header = curl_slist_append(header, (i.key() + ": " + i.value()).toStdString().c_str());
    }
}

void KTools::Curl::setHeader(const QMap<QString, QString> &chunk)
{
    /*QMap<QString, QString>::const_iterator end = chunk.end();
    for (QMap<QString, QString>::const_iterator i = chunk.begin(); i != end; ++i)
    {
        headerData[i.key()] = i.value();
    }*/
    headerData = chunk;
    generateHeader();
}

void KTools::Curl::setHeaderLine(const std::pair<QString, QString> &headerLine)
{
    headerData[headerLine.first] = headerLine.second;
    generateHeader();
}

void KTools::Curl::delHeaderLine(const QString &key)
{
    headerData.remove(key);
    generateHeader();
}

void KTools::Curl::setErrFile(const QString &path, const QString &fileName)
{
    stdErr = fopen((path + "/" + fileName).toStdString().c_str(), "w");
    errFileSetted = true;
    verboseOutput = true;
}

void KTools::Curl::unsetErrFile()
{
    if (errFileSetted)
    {
        fclose(stdErr);
        errFileSetted = false;
        verboseOutput = false;
    }
}

void KTools::Curl::addHandle(const QString &handleName)
{
    CURL* newHandle = curl_easy_init();
    handlesList[handleName] = newHandle;
}

void KTools::Curl::swichHandle(const QString &handleName)
{
    gCurlHandle = handlesList[handleName];
}

void KTools::Curl::setCookie()
{
    /*std::string str = "";
    struct curl_slist *next = cookies;
    while (next)
    {
        str += next->data;
        str += "\n";
        next = next->next;
    }
    curl_easy_setopt(gCurlHandle, CURLOPT_COOKIELIST, str.c_str());*/
    curl_easy_setopt(gCurlHandle, CURLOPT_COOKIEFILE, (cookiePath + cookieFileName).toStdString().c_str());
}
