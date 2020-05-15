import QtQuick 2.0
import QtQuick.Controls 2.5
import "../../js/Tools.js" as Tools

Rectangle {
    id: globalRoot
    anchors.fill: parent
    property var windowsObects: []
    property int currentWindowId: 0
    property int comboBoxCounter: 0
    Rectangle {
        id: leftBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: Tools.cutNumberByPercents(parent.width, 15)
        ComboBox {
            id: selectTypeComboBox
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            model: comboBoxListModel
            textRole: "text"
            valueRole: "id"
            onCurrentIndexChanged: comboBoxSelectionChanged()
        }

        KawaiHorisontalLine {
            id: leftBarDivisionLine
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: selectTypeComboBox.bottom
            anchors.topMargin: 20
        }

        ListView {
            id: leftBarListView
            model: leftBarListViewModelItem
            delegate: leftBarListViewDelegateItem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: leftBarDivisionLine.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 5
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
            id: comboBoxListModel
        }
        ListModel {
            id: leftBarListViewModelItem
        }
        Component {
            id: leftBarListViewDelegateItem
            KawaiButton {
                anchors.left: parent.left
                anchors.right:  parent.right
                anchors.margins: 5
                labelText: buttonText
                onClicked: callWindow(windowId)
            }
        }

    }

    Rectangle {
        id: contentRectangle
        anchors.left: leftBar.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

    function addItems(itemParams)
    {
        for (var i = 0 ; itemParams[i]; i++)
        {
            var winId = createAndAddObject(itemParams[i][1], itemParams[i][0])
            leftBarListViewModelItem.append({buttonText: itemParams[i][0], windowId:winId})
        }
    }
    function createAndAddObject(path, name)
    {
        var len = windowsObects.length
        var obj = Qt.createComponent(path)
        if (obj.status == Component.Error)
            console.log(obj.errorString())
        windowsObects.push([
            comboBoxCounter, // sectionId
            obj.createObject(contentRectangle, {visible: false}),
            len, // windowId
            name // buttonText
        ]);
        return len
    }
    function callWindow(windowId)
    {
        windowsObects[currentWindowId][1].visible = false;
        windowsObects[windowId][1].visible = true;
        currentWindowId = windowId
    }
    function addItemsAndSections(params)
    {
        for (var i = 0; params[i]; i++)
        {
            comboBoxListModel.append({text:params[i][0], id:comboBoxCounter})
            addItems(params[i][1])
            comboBoxCounter++
        }
    }
    function comboBoxSelectionChanged()
    {
        leftBarListViewModelItem.clear()
        var newId = comboBoxListModel.get(selectTypeComboBox.currentIndex).id
        for (var i = 0; windowsObects[i]; i++)
        {
            if (windowsObects[i][0] === newId)
                leftBarListViewModelItem.append({buttonText: windowsObects[i][3], windowId: windowsObects[i][2]})
        }
    }
}
