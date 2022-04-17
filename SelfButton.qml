import QtQuick 2.0

Rectangle {
    id: root
    property alias text: label.text
    signal clicked

    width: 140; height: 40
    border.color: '#409eff'
    radius: 5

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 20
        text: "自定义按钮"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            root.color = '#409eff'
        }
        onExited: {
            root.color = '#ffffff'
        }
        onClicked: {
            root.clicked()
        }
        onPressed: {
            root.color = '#4050ff'
        }
        onReleased: {
            containsMouse ? root.color = '#409eff' : root.color = '#ffffff'
        }
    }
}
