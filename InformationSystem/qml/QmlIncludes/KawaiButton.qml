import QtQuick 2.0
import QtQuick.Controls 2.2

Button {
    id:root
    property string labelText: "Sample text"
    property string onPressedColor: "Black"
    property string onHoveredColor: "Gray"
    property string standartColor: "#dadada"
    property string borderColorC: "Black"
    property string onPressedBorderColor: "Black"
    property string onHoveredBorderColor: "Black"
    property string labelTextColor: "Black"
    //property string onPressedLabelTextColor: "Black"
    //property string onHoveredLabelTextColor: "Black"
    property int borderWidthC: 1
    property int borderRadiusC: 5
    property int heightC: 40
    property int widthC: 100
    property int labelTextWeight: Font.Normal
    background: Rectangle {
        id: buttonRectangle
        implicitWidth: widthC
        implicitHeight: heightC
        border.color: borderColorC
        border.width: borderWidthC
        radius: borderRadiusC
        color: standartColor
        Text {
            id: buttonLabel
            anchors.centerIn: parent
            text: labelText
            color: labelTextColor
            font.weight: labelTextWeight
        }
    }
    states: [
        State {
            name: "onPressedButtonRectangle"
            when: root.pressed == true
            PropertyChanges {
                target: buttonRectangle
                color: onPressedColor
                border.color: onPressedBorderColor
            }
        },
        State {
            name: "onHoveredButtonRectangle"
            when: root.hovered == true
            PropertyChanges {
                target: buttonRectangle
                color: onHoveredColor
                border.color: onHoveredBorderColor
            }
        }
    ]
}
