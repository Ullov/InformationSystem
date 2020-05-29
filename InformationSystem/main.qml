import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.2
import "qml/QmlIncludes"
import "qml"

Window {
    visible: true
    width: 800
    height: 500
    title: qsTr("Information system")
    //visibility: "Maximized"

    //Component.onCompleted: inter.getData({url: "https://www.youtube.com/channel/UCrG8mytOLrC_t5vu7To4ajA"})
    KawaiLabelInputFieldCombination {
        id: addressInput
        anchors.left: parent.left
        anchors.right: submitButton.left
        anchors.top: parent.top
        anchors.margins: 5
        klifcLabel.text: "Название видео"
        klifcLabel.font.family: "Segoe UI"
        klifcTextField.font.family: "Segoe UI"
        //klifcTextField.placeholderText: "Введи адрес канала"
    }
    KawaiButton {
        id: submitButton
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 5
        labelText: "Найти ключевые слова"
        widthC: 150
        onClicked: {
            inter.getData({url: addressInput.klifcTextField.text})
            resultText.text = "Идёт обработка данных"
            //videosListModel.clear()
            keywordsCounter = {}
        }
    }
    Rectangle {
        anchors.top: addressInput.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 25
        border.color: "Black"
        border.width: 2
        radius: 2
        ScrollView{
            id: resultScroll
            anchors.fill: parent
            anchors.margins: 5
            TextArea {
                id: resultText
                wrapMode: TextArea.WordWrap
                font.family: "Segoe UI"
                readOnly: true
            }
        }
    }


    property var keywordsCounter: ({})
    Connections {
        target: inter
        onYtData: {
            resultText.text = "Данные проанализированы"
            videosListModel.clear()
            for (var i = 0; data[i]; i++)
            {
                videosListModel.append({inf: {info: data[i]}})
            }
        }
        onSingleData: {
            //videosListModel.append({inf: data})
            //data["keywords"]
            for (var i = 0; data["keywords"][i]; i++)
            {
                if (keywordsCounter[data["keywords"][i]])
                    keywordsCounter[data["keywords"][i]] += 1
                else
                    keywordsCounter[data["keywords"][i]] = 1
            }
            var str = ""
            var keys = Object.keys(keywordsCounter)
            for (var i = 0; keys[i]; i++)
            {
                str += keys[i]
                if (keys[i + 1])
                    str += ","
            }
            resultText.text = str
        }
    }
}
