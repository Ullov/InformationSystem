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

    TextArea {
        id: infoText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: addressInput.bottom
        height: 40
        font.family: "Segoe UI"
        readOnly: true
        selectByMouse: true
    }

    KawaiLabelInputFieldCombination {
        id: addressInput
        anchors.left: parent.left
        anchors.right: submitButton.left
        anchors.top: parent.top
        anchors.margins: 5
        klifcLabel.text: "Название видео"
        klifcLabel.font.family: "Segoe UI"
        klifcTextField.font.family: "Segoe UI"
    }
    KawaiButton {
        id: submitButton
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 5
        labelText: "Найти ключевые слова"
        widthC: 150
        onClicked: {
            if (addressInput.klifcTextField.text != "")
            {
                if (allDataPiecesHere)
                {
                    inter.getData({url: addressInput.klifcTextField.text})
                    infoText.text = "Идёт обработка данных"
                    resultText.text = ""
                    lineByLineResultText.text = ""
                    keywordsCounter = {}
                }
            }
            else
            {
                infoText.text = "Пустой ввод неприемлем"
            }
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
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: dividingLine.top
            anchors.margins: 5
            TextArea {
                id: resultText
                wrapMode: TextArea.WordWrap
                font.family: "Segoe UI"
                readOnly: true
                selectByMouse: true
            }
        }
        KawaiHorisontalLine {
            id: dividingLine
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
        }
        ScrollView {
            id: lineByLineResultScroll
            anchors.top: dividingLine.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 5
            TextArea {
                id: lineByLineResultText
                wrapMode: TextArea.WordWrap
                font.family: "Segoe UI"
                readOnly: true
                selectByMouse: true
            }
        }
    }

    property var keywordsCounter: ({})
    property bool allDataPiecesHere: true
    Connections {
        target: inter
        onSingleData: {
            allDataPiecesHere = data["last"]
            if (data["last"])
                infoText.text = ""

            for (var i = 0; data["keywords"][i]; i++)
            {
                if (keywordsCounter[data["keywords"][i]])
                    keywordsCounter[data["keywords"][i]] += 1
                else
                    keywordsCounter[data["keywords"][i]] = 1
            }
            var str = ""
            var str2 = ""
            var keys = Object.keys(keywordsCounter)
            for (var i = 0; keys[i]; i++)
            {
                str += keys[i]
                str2 += keys[i]
                if (keys[i + 1])
                {
                    str += ", "
                    str2 += ",\n"
                }
            }
            resultText.text = str
            lineByLineResultText.text = str2
        }
    }
}
