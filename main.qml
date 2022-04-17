import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("桌面端本地播放器")

    // 标题栏
    Header {
        id: header
        text: "播放器登陆界面"

        Rectangle {
            width: 214
            height: 60
            radius: 10

            anchors.right: parent.right
            anchors.rightMargin: 26
            anchors.top: parent.top
            anchors.topMargin: 11

            Rectangle {
                width: 143
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
        }
    }

    // 输入表单
    Column {
        id: form

        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20

        spacing: 5
        Input {
            id: userName
        }
        Input {
            id: password
        }
    }

    SelfButton {
        id: loginBtn
        anchors.top: form.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: '登录'
        onClicked: {
            search.focus = false
            search.text = '搜索框输入内容'
        }
    }
}
