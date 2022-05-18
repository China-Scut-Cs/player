import QtQuick 2.0
import QtQuick.Controls

Rectangle {
    id: header
    property alias text: label.text
    width: parent.width
    height: 80
    color: '#409eff'

    Text {
        id: label
        width: parent.width
        height: parent.height
        font.pixelSize: 30
        anchors.left: parent.left
        anchors.leftMargin: 45
        verticalAlignment: Text.AlignVCenter
        text: '自定义标题'
        color: '#ffffff'
    }
}
