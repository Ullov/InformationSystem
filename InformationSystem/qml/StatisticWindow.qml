import QtQuick 2.0
import "QmlIncludes"

Rectangle {
    //Component.onCompleted: inter.getData({url: "https://www.youtube.com/channel/UCrG8mytOLrC_t5vu7To4ajA"})
    KawaiLabelInputFieldCombination {
        id: addressInput
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
    }

    Text {
        id: resultText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: addressInput.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 5
    }
}
