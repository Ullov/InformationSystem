#ifndef KTOOLSQMLINTERFACE_H
#define KTOOLSQMLINTERFACE_H

#include <QObject>
#include "options.h"

class KToolsQmlInterface : public QObject
{
    Q_OBJECT
public:
    KToolsQmlInterface();

public slots:
    // Start OptionsHandler
    void optionsHandlerSetRootProgramPath(const QString path);
    void optionsHandlerEmitRootProgramPath();
    void optionsHandlerSlotSetParam(const QString pathToParam, const QString param);
    void optionsHandlerSlotGetParam(const QString pathToParam);
    // End OptionsHandler

public:
    static KToolsQmlInterface *obj;

signals:
    // Start OptionsHandler
    void optionsHandlerSendRootProgramPath(const QString path);
    void optionsHandlerSignalParam(const QString pathToParam, const QVariant param);
    // End OptionsHandler
};

#endif // KTOOLSQMLINTERFACE_H
