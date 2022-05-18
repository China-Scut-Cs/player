import QtQuick 2.12

// gridView 展示内容控件
Rectangle {
    id: itemWrapper
    width: 267; height: 225
    radius: 6

    property alias coverSource: img.source
    property string videoSource: ''
    property alias title: title.text
    property alias author: author.text
    property alias date: date.text

    signal doubleClicked
    signal clicked

    // 图片区域
    Rectangle {
        id: itemImg
        width: 267; height: 173
        radius: 6
        Image {
            id: img
            width: parent.width
            height: parent.height
            fillMode: Image.PreserveAspectCrop
        }
    }

    // 标题
    Text {
        id: title
        width: parent.width
        height: 30
        anchors.top: itemImg.bottom
        anchors.topMargin: 5
        leftPadding: 10
        text: '自定义标题'
        font.pixelSize: 25
    }

    // 作者姓名
    Text {
        id: author
        width: contentWidth
        height: 15
        anchors.top: title.bottom
        anchors.topMargin: 4
        anchors.left: parent.left
        leftPadding: 10
        text: '作者姓名'
        font.pixelSize: 15
        color: '#797979'
    }

    // 添加时间
    Text {
        id: date
        width: contentWidth
        height: 15
        anchors.top: title.bottom
        anchors.topMargin: 4
        anchors.left: author.right
        anchors.leftMargin: 5
        leftPadding: 10
        text: '添加日期'
        font.pixelSize: 15
        color: '#797979'
    }
}
