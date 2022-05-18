import QtQuick 2.12
import QtQuick.Controls

Rectangle {
    id: root
    anchors.fill: parent

    GridView {
        id: videoGrid
        anchors.fill: parent
        anchors.left: parent.left
        anchors.leftMargin: 45
        anchors.top: parent.top
        anchors.topMargin: 45
        cellWidth: 308; cellHeight: 277
        model: videoModel
        delegate: videoCmp
    }

    ListModel {
        id: videoModel
        ListElement {
            cover: './asset/image/noFile.jpg'
            head: '自定义标题1'
            name: '1'
            time: '2022-5-1'
        }
        ListElement {
            cover: './asset/image/noFile.jpg'
            head: '自定义标题2'
            name: '2'
            time: ''
        }
        ListElement {
            cover: './asset/image/noFile.jpg'
            head: ''
            name: '3'
            time: ''
        }
        ListElement {
            cover: './asset/image/noFile.jpg'
            head: ''
            name: ''
            time: ''
        }
        ListElement {
            cover: './asset/image/noFile.jpg'
            head: ''
            name: ''
            time: ''
        }
        ListElement {
            cover: './asset/image/noFile.jpg'
            head: '视频6'
            name: ''
            time: ''
        }
    }

    Component {
        id: videoCmp
        Item {
            id: wrapper
            width: videoGrid.cellWidth
            height: videoGrid.cellHeight

            AVItem {
                id: clickedItem
                anchors.fill: parent
                coverSource: cover
                title: head
                author: name
                date: time
            }

            Popup {
                id: popup
                anchors.centerIn: Overlay.overlay
                width: 100
                height: 100
                Text {
                    id: popupTitle
                    text: clickedItem.title
                }
                Text {
                    id: popupAuthor
                    anchors.top: popupTitle.bottom
                    text: clickedItem.author
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onDoubleClicked: (mouse) => {
                    if(mouse.button === Qt.LeftButton) {
                        // 左键双击
                        home.visible = false
                        var videoPlayPage = Qt.createComponent("VideoPlay.qml")
                                         .createObject(mainWin, {avItem: clickedItem})
                    }
                }
                onClicked: (mouse) => {
                    if(mouse.button === Qt.RightButton) {
                        // 右键单击
                        console.log('右键单击' + clickedItem.title)
                        popup.open()
                    }
                }
            }
        }
    }
}
