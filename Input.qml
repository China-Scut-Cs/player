import QtQuick 2.12
import QtQuick.Controls

Rectangle {
    id: inputContent

    property alias source: icon.source
    property alias text: inputName.text
    property alias echoMode: input.echoMode

    signal textEdited

    width: 340
    height: 47
    Text {
        id: inputName;
        width: 74
        height: 47
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: '名称： '
        font.pixelSize: 20
        color: '#409eff'
    }

    Rectangle {
        id: inputWrapper
        width: 251; height: 47
        anchors.left: inputName.right
        anchors.leftMargin: 15
        border.color: '#797979'
        radius: 6

        Label {
            width: 24
            height: 30
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.top: parent.top
            anchors.topMargin: 9
            Image {
                id: icon
                anchors.fill: parent
                source: ''
                fillMode: Image.PreserveAspectFit
            }
        }

        Rectangle {
            id: inputBorder
            width: 186
            height: 35
            anchors.top: parent.top
            anchors.topMargin: 6
            anchors.right: parent.right
            anchors.rightMargin: 14
            radius: 3
            border.color: '#797979'

            TextInput {
                id: input
                anchors.fill: parent
                verticalAlignment: TextInput.AlignVCenter
                leftPadding: 10
                selectByMouse: true
                font.pixelSize: 13
                echoMode: TextInput.Normal
                onTextEdited: {
                    inputContent.textEdited()
                }
            }
        }
    }
}
