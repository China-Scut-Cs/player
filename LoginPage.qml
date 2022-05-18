import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls

Rectangle {
    id: loginPage
    // 标题栏
    Header {
        id: header
        text: "播放器登陆界面"
    }

    Rectangle {
        id: body
        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        Rectangle {
            id: formWrap
            anchors.centerIn: parent
            width: 424
            height: 315
            border.color: '#797979'
            radius: 6

            // 输入表单
            Column {
                id: form
                anchors.top: parent.top
                anchors.topMargin: 86
                anchors.left: parent.left
                anchors.leftMargin: 50
                spacing: 21
                Input {
                    id: userName
                    text: '用户名：'
                    source: './image/user-solid.svg'
                }
                Input {
                    id: password
                    echoMode: TextInput.Password
                    text: '密码： '
                    source: './image/lock-solid.svg'
                }
            }

            Row {
                id: buttonGroup
                anchors.top: form.bottom
                anchors.topMargin: 19
                anchors.left: parent.left
                anchors.leftMargin: 50
                spacing: 52
                PrimaryButton {
                    id: login
                    text: '登录'
                    onClicked: {
                        loader.sourceComponent = home
                    }
                }
                PrimaryButton {
                    id: register
                    text: '注册'
                    onClicked: {
                        loader.sourceComponent = regPage
                    }
                }
            }
        }
    }
}
