import QtQuick 2.0

Rectangle {
    id: errWrap
    property alias text: err.text
    width: 340
    height: 53
    Text {
        id: err
        anchors.fill: parent
        color: '#f56c6c'
        text: '错误提示信息'
        font.pixelSize: 25
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
