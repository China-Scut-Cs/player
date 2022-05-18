import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls

Rectangle {
    id: regPage
    // 标题栏
    Header {
        id: header
        text: "注册用户"
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
                anchors.topMargin: 31
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 26
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
                    onTextEdited: {
                        pwdInfo.visible = true
                    }
                }
                Input {
                    id: comfirm
                    echoMode: TextInput.Password
                    text: '确认密码：'
                    source: './image/key-solid.svg'
                }
            }

            Label {
                id: pwdInfo
                width: contentWidth
                height: contentHeight
                anchors.left: parent.left
                anchors.leftMargin: 140
                anchors.top: parent.top
                anchors.topMargin: 155
                text: '6-10字符，只包含数字、字母和下划线'
                font.pixelSize: 14
                visible: false
            }

            Label {
                id: errInfo
                width: contentWidth
                height: contentHeight
                anchors.left: parent.left
                anchors.leftMargin: 140
                anchors.top: parent.top
                anchors.topMargin: 227
                text: '密码与确认密码不符！请重试！'
                font.pixelSize: 14
                color: '#f56c6c'
            }

            Row {
                id: buttonGroup
                anchors.top: form.bottom
                anchors.topMargin: 38
                anchors.left: parent.left
                anchors.leftMargin: 50
                spacing: 52
                PrimaryButton {
                    id: register
                    text: '注册'
                }
                PrimaryButton {
                    id: cancle
                    text: '取消'
                    onClicked: {
                        loader.sourceComponent = loginPage
                    }
                }
            }
        }
    }
}
