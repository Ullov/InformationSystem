import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.2
import "qml/QmlIncludes"
import "qml"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Information system")
    visibility: "Maximized"

    //Component.onCompleted: inter.getData({url: "https://www.youtube.com/channel/UCrG8mytOLrC_t5vu7To4ajA"})
    KawaiLabelInputFieldCombination {
        id: addressInput
        anchors.left: parent.left
        anchors.right: submitButton.left
        anchors.top: parent.top
        anchors.margins: 5
        klifcLabel.text: "Адрес канала"
        klifcLabel.font.family: "Segoe UI"
        klifcTextField.font.family: "Segoe UI"
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
            videosListModel.clear()
            keywordsCounter = {}
        }
    }
    ScrollView{
        id: resultScroll
        anchors.top: addressInput.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        width: 300
        TextArea {
            id: resultText
            wrapMode: TextArea.WordWrap
            font.family: "Segoe UI"
            readOnly: true
        }

    }
    ListView {
        id: videosListView
        anchors.left: parent.left
        anchors.right: resultScroll.left
        anchors.top: addressInput.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: videosListModel
        delegate: videosDelegate
        clip: true
        ScrollBar.vertical: ScrollBar {
            id: scrollBarItem
            policy: ScrollBar.AlwaysOn
        }
        Keys.onUpPressed: scrollBarItem.decrease()
        Keys.onDownPressed: scrollBarItem.increase()
        spacing: 3
    }
    ListModel {
        id: videosListModel
    }

    Component {
        id: videosDelegate
        InfoAboutVideo {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            info: inf
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
            videosListModel.append({inf: data})
            data["keywords"]
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
                str += keys[i] + "\t\t" + keywordsCounter[keys[i]] + "\n"
            }
            resultText.text = str
        }
    }
}
