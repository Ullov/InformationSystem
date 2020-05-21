import QtQuick 2.5
import QtQuick.Controls 2.2
import "QmlIncludes"

Rectangle {
    id: root
    property var info: []
    property bool folded: true
    property int foldedHeight: 65
    property int unfoldedHeight: parent.height / 2
    property int animationDuration: 300
    border.color: "Grey"
    border.width: 3
    radius: 5
    clip: true

    Rectangle {
        id: averageInfoRectangle
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
        clip: true
        anchors.margins: 5
        TextArea {
            id: averageInfoLabel
            anchors.fill: parent
            anchors.margins: 5
            wrapMode: TextArea.WordWrap
            font.family: "Segoe UI"
            readOnly: true
        }
    }

    KawaiButton {
        id: button
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 2
        width: 20
        height: 20
        labelText: "+"
        onClicked: {
            if (folded)
            {
                folded = false
                root.height = unfoldedHeight
            }
            else
            {
                folded = true
                root.height = foldedHeight
            }
        }
    }

    ListView {
        id: commentsListView
        anchors.top: averageInfoRectangle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: commentsListModel
        delegate: commentDelegateItem
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
        id: commentsListModel
    }

    Component {
        id: commentDelegateItem
        Rectangle {
            id: commentInnerDelegate
            anchors.left: parent.left
            anchors.right: parent.right
            height: commentText.paintedHeight + 30
            border.color: "Black"
            TextArea {
                id: commentText
                anchors.fill: parent
                anchors.margins: 5
                text: str
                wrapMode: TextArea.WordWrap
                font.family: "Segoe UI"
                readOnly: true
            }
        }
    }

    onInfoChanged: {
        var allLikes = 0
        for (var i = 0; info["comments"][i]; i++)
        {
            var str = ""
            str += "Имя: " + info["comments"][i]["opName"] + "\n"
            str += "Лайки: " + info["comments"][i]["likes"] + "\n"

            if (info["comments"][i]["replies"])
                str += "Количество ответов: " + info["comments"][i]["replies"] + "\n"

            str += "Текст комментария: " + info["comments"][i]["text"]
            commentsListModel.append({str: str})
            allLikes += info["comments"][i]["likes"]
        }
        var res = "";
        res += "Название видео: " + info["title"] + "\n"
        res += "Ключевые слова:" + info["keywords"] + "\n"
        res += "Всего лайков: " + allLikes + "\n"
        res += "Отношение лайков и комментариев: " + (allLikes / i) + "\n"
        averageInfoLabel.text = res
    }

    Component.onCompleted: {
        if (!folded)
        {
            folded = false
            root.height = unfoldedHeight
        }
        else
        {
            folded = true
            root.height = foldedHeight
        }
    }
}
