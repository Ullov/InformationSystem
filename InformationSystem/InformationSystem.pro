QT += quick

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cpp_h/KTools/HtmlAstMaker/object.cpp \
        cpp_h/KTools/HtmlAstMaker/tag.cpp \
        cpp_h/KTools/StaticVarsDeclarations.cpp \
        cpp_h/KTools/converter.cpp \
        cpp_h/KTools/curl.cpp \
        cpp_h/KTools/exforstring.cpp \
        cpp_h/KTools/file.cpp \
        cpp_h/KTools/fileidentifier.cpp \
        cpp_h/KTools/kawaiimageprovider.cpp \
        cpp_h/KTools/ktoolsqmlinterface.cpp \
        cpp_h/KTools/log.cpp \
        cpp_h/KTools/options.cpp \
        cpp_h/interfaceforqml.cpp \
        cpp_h/parserclass.cpp \
        main.cpp \
        youtube.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cpp_h/KTools/HtmlAstMaker/object.h \
    cpp_h/KTools/HtmlAstMaker/tag.h \
    cpp_h/KTools/converter.h \
    cpp_h/KTools/curl.h \
    cpp_h/KTools/enums.h \
    cpp_h/KTools/exforstring.h \
    cpp_h/KTools/file.h \
    cpp_h/KTools/fileidentifier.h \
    cpp_h/KTools/kawaiimageprovider.h \
    cpp_h/KTools/ktoolsqmlinterface.h \
    cpp_h/KTools/log.h \
    cpp_h/KTools/options.h \
    cpp_h/interfaceforqml.h \
    cpp_h/parserclass.h \
    youtube.h


#Curl
LIBS += -L$$PWD/libs/curl-x64/lib/dll/ -llibcurl.dll
INCLUDEPATH += $$PWD/libs/curl-x64/include
DEPENDPATH += $$PWD/libs/curl-x64/include
