import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.2
import "qml/QmlIncludes"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Information system")
    visibility: "Maximized"

    /*KawaiSwitchableWindows {
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
    }*/
    //Component.onCompleted: inter.getData({url: "https://www.youtube.com/channel/UCrG8mytOLrC_t5vu7To4ajA"})
    KawaiLabelInputFieldCombination {
        id: addressInput
        anchors.left: parent.left
        anchors.right: submitButton.left
        anchors.top: parent.top
        anchors.margins: 5
        klifcLabel.text: "Адрес канала"
        klifcTextField.placeholderText: "Введи адрес канала"
    }
    KawaiButton {
        id: submitButton
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 5
        onClicked: {
            inter.getData({url: addressInput.klifcTextField.text})
            resultText.text = "Идёт обработка данных"
        }
    }
ScrollView{
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: addressInput.bottom
    anchors.bottom: parent.bottom
    anchors.margins: 5
    TextArea {
        id: resultText
        wrapMode: TextArea.WordWrap
        font.family: "Segoe UI"
        readOnly: true
    }
}

    Connections {
        target: inter
        onYtData: {
            //resultText.text = data
            var str = ""
            for (var i = 0; data[i]; i++)
            {
                var allLikes = 0;
                var commentsText = "";
                var j
                for (j = 0; data[i][j]; j++)
                {
                    allLikes += data[i][j]["likes"]
                    commentsText += "\tДанные по комментарию #" + j + "\n"
                    commentsText += "\tАвтор: " + data[i][j]["opName"] + "\n"
                    commentsText += "\tЛайки: " + data[i][j]["likes"] + "\n"
                    commentsText += "\tТекст: " + data[i][j]["text"] + "\n\n"
                }
                str += "Данные по видео #" + i + "\n"
                str += "Всего лайков: " + allLikes + "\n"
                str += "Среднеее количество лайков: " + (allLikes / j) + "\n"
                str += commentsText
            }
            resultText.text = str
        }
    }
}
