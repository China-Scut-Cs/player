import QtQuick 2.12
import QtQuick.Controls

Rectangle {
    id: root
    width: 450
    height: 60
    radius: 10
    border.color: '#797979'

    signal click
    signal pressEnter

    Label {
        width: 30
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: 17
        anchors.top: parent.top
        anchors.topMargin: 15
        Image {
            id: icon
            anchors.fill: parent
            source: './asset/image/search.svg'
            fillMode: Image.PreserveAspectFit
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                root.click()
                console.log(search.displayText)
            }
        }
    }

    Rectangle {
        width: 379
        height: 30
        radius: 5
        border.color: '#797979'
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 15

        TextInput {
            id: search
            anchors.fill: parent
            leftPadding: 10
            verticalAlignment: TextInput.AlignVCenter
            selectByMouse: true
            font.pixelSize: 13
        }
    }

    Keys.onEnterPressed: {
        root.pressEnter()
        console.log(search.displayText)
    }
}
