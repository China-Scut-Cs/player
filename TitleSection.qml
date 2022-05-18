import QtQuick
import QtQuick.Controls

// 标题栏
Rectangle {
    id: topControl
    anchors.left: parent.left
    anchors.right: parent.right
    height: 50
    color: '#ebebeb'
    z: 10

    signal click

    // 返回按钮
    Text {
        id: history
        width: 23; height: 25
        anchors.top: parent.top
        anchors.topMargin: 13
        anchors.left: parent.left
        anchors.leftMargin: 20
        z: 5
        Image {
            id: historyImg
            anchors.fill: parent
            source: "./asset/image/arrow-left-solid.svg"
            fillMode: Image.PreserveAspectFit
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: "PointingHandCursor"
            onClicked: {
                topControl.click()
            }
        }
    }
    // 标题部分
    Text {
        id: titleContent
        anchors.left: parent.left
        anchors.leftMargin: 66
        anchors.top: parent.top
        anchors.topMargin: 13
        font.pixelSize: 20
        text: avItem.title
    }
    // 播放列表
    ToolButton {
        id: drawerBtn
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 8
        text: '\u2630'
        font.pixelSize: Qt.application.font.pixelSize * 1.6
        z: 5
        onClicked: drawer.open()
    }
    Drawer {
        id: drawer
        width: 150
        height: mainWin.height
        dragMargin: 10
        edge: Qt.RightEdge

        Column {
            anchors.fill: parent
            ItemDelegate {
                text: '一个抽屉标签'
                width: parent.width
                height: 60
                onClicked: {
                    drawer.close()
                }
            }
            ItemDelegate {
                text: '一个抽屉标签'
                width: parent.width
                height: 20
                onClicked: {
                    drawer.close()
                }
            }
            ItemDelegate {
                text: '一个抽屉标签'
                width: parent.width
                height: 20
                onClicked: {
                    drawer.close()
                }
            }
        }

        background: Rectangle {
            x: parent.width - 1
            width: 1
            height: parent.height
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            hover5SecTimer.stop()
            hoverArea.enabled = false
            topControl.visible = true
        }
        onExited: {
            hover5SecTimer.start()
            hoverArea.enabled = true
        }
    }
}
