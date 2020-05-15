#include "parserclass.h"

ParserClass::ParserClass()
{
    defExt = new KTools::FileIdentifier();
    cc = new KTools::Curl();
}

ParserClass::~ParserClass()
{
    delete defExt;
    delete cc;
}

void ParserClass::recExtractJson(const QJsonObject &rootObject, QString offset, QString &data)
{
    QString tmp;
    QStringList keysChain = rootObject.keys(); // Number; Null; Array; String; Object(aka array); Bool;
    for (int i = 0; i < keysChain.length(); i++)
    {
        if (rootObject.value(keysChain[i]).isString()) // is string
            data += offset + keysChain[i] + ": " + rootObject.value(keysChain[i]).toString() + '\n';
        else if (rootObject.value(keysChain[i]).isObject()) // is object
        {
            data += offset + "\\+" + keysChain[i] + '\n';
            recExtractJson(rootObject.value(keysChain[i]).toObject(), offset + '\t', data);
        }
        else if (rootObject.value(keysChain[i]).isNull()) // is null
            data += offset + keysChain[i] + ": Is Null\n";
        else if (rootObject.value(keysChain[i]).isBool()) // is bool
        {
            if (rootObject.value(keysChain[i]).toBool())
                tmp = offset + keysChain[i] + ": True\n";
            else
                tmp = offset + keysChain[i] + ": False\n";
            data += tmp;
        }
        else if (rootObject.value(keysChain[i]).isArray()) // is array
        {
            data += offset + "\\+" + keysChain[i] + '\n';
            QJsonValue val = rootObject.value(keysChain[i]);
            QJsonArray arr = val.toArray();
            int c = 0;
            offset += '\t';
            foreach (const QJsonValue &v, arr)
            {
                if (v.isBool())
                {
                    if (v.toBool())
                        tmp = offset + QString::number(c) + ": True\n";
                    else
                        tmp = offset + QString::number(c) + ": False\n";
                    data += tmp;
                }
                else if (v.isDouble())
                    data += offset + QString::number(c) + ": " + v.toString() + '\n';
                else if (v.isNull())
                    data += offset + QString::number(c) + ": Is Null\n";
                else if (v.isObject())
                {
                    data += offset + "\\+" + QString::number(c) + '\n';
                    recExtractJson(v.toObject(), offset + '\t', data);
                }
                else if (v.isString())
                    data += offset + QString::number(c) + ": " + v.toString() + '\n';
                c++;
            }
            offset.chop(1);
        }
        else if (rootObject.value(keysChain[i]).isDouble()) // is double (aka number)
            data += offset + keysChain[i] + ": " + rootObject.value(keysChain[i]).toVariant().toString() + '\n';
    }
    offset.chop(1);
}

void ParserClass::writeJsonDataInFile(const QJsonObject &object, const QString &path, const QString &fileName)
{
    QString jData;
    jData = QString();
    recExtractJson(object, "", jData);
    KTools::File::writeFile(jData.toUtf8(), path, fileName);
}

QJsonObject ParserClass::downloadJson(const QString url)
{
    QByteArray result = cc->performing(url.toUtf8());
    QJsonObject object = KTools::Converter::convert<QString, QJsonObject>(result);
    return object;
}

void ParserClass::downloadAndWriteFile(const QString &url, const QString &path, const QString &fileName)
{
    QByteArray result = cc->performing(url.toUtf8());
    KTools::File::writeFile(result, path, fileName);
}

QJsonArray ParserClass::downloadJsonAsArray(const QString &url)
{
    QByteArray result = cc->performing(url.toUtf8());
    QJsonArray arr = KTools::Converter::convert<QString, QJsonArray>(result);
    return arr;
}

void ParserClass::delay(const int &seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void ParserClass::downloadAndWriteFileWithDefinedExtension(const QString &url, const QString &path, const QString &fileName)
{
    QByteArray fileString = cc->performing(url.toUtf8());
    QString extension = defineExtension(fileString);
    KTools::File::writeFile(fileString, path, fileName + extension);
}

QString ParserClass::defineExtension(const QByteArray &file)
{
    QStringList tmp = defExt->identifyFileFromString(file);
    return tmp[0];
}

QVector<QJsonObject> ParserClass::extractJsonObjectFromText(const QString &text)
{
    QString pattern;
    QVector<QString> regexResult;
    QVector<QJsonObject> objects;
    pattern = "({\".+})+"; // =*({(\")(.)+})+

    KTools::ExForString::executeRegex(text, pattern, regexResult);
    for (int i = 0; i < regexResult.size(); i++)
    {
        QString tmp = regexResult[i];
        QJsonObject obj = KTools::Converter::convert<QString, QJsonObject>(tmp);
        if (!obj.isEmpty())
            objects.push_back(obj);
    }
    return objects;
}

void ParserClass::writeInfoLog(const QString &message)
{
    KTools::Log::writeCustomLog(message, KTools::Options::parsersNames[parserType], KTools::Enums::LogType::Info, rootPath, logFile);
}

void ParserClass::setParserType(const KTools::Enums::Parsers type)
{
    parserType = type;
    basePath = KTools::Options::rootProgramPath + '/' + KTools::Options::parsersWritePathes[type];
    parserName = KTools::Options::parsersNames[type];
    cc->downloaderType = type;
}

void ParserClass::endDownloadingFunction(const int parserMode, const QJsonObject &data, const QVector<QByteArray> &binaryContent)
{
    QList<int> mode;
    mode.push_back(static_cast<int>(parserType));
    mode.push_back(parserMode);
    emit downloadingFinished(mode, data, binaryContent);
}
