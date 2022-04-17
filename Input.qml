import QtQuick 2.12
import QtQuick.Controls

Rectangle {
    id: inputWrapper
    width: 251; height: 47
    border.color: '#797979'
    radius: 6

    TextInput {
        id: input
        anchors.fill: parent
        leftPadding: 47
        verticalAlignment: TextInput.AlignVCenter
        selectByMouse: true
        font.pixelSize: 13
    }
}
