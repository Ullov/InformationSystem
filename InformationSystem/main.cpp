#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "cpp_h/interfaceforqml.h"
#include <curl/curl.h>

void registerTypesForQml()
{
    qRegisterMetaType<QVector<QByteArray>>("VectorByteArray");
    KTools::Enums::registerTypesForQml();
}

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    registerTypesForQml();
    curl_global_init(CURL_GLOBAL_ALL);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    InterfaceForQml *in = new InterfaceForQml();
    engine.rootContext()->setContextProperty("inter", in);
    engine.load(url);

    return app.exec();
}
