import QtQuick 2.13
import QtQuick.Window 2.13
import "qml/QmlIncludes"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Information system")
    visibility: "Maximized"

    KawaiSwitchableWindows {
        id: window
    }
    Component.onCompleted: {
        var obj = [
            [
                "InformationSystem",
                [
                    ["Main window", "qrc:/qml/StatisticWindow.qml"]
                ]
            ]
        ]
        window.addItemsAndSections(obj)
    }
}
