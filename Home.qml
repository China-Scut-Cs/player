import QtQuick 2.12
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: home
    Header {
        id: header
        text: '桌面端本地播放器'
        z: 2
        //搜索栏
        SearchInput {
            id: search
            anchors.centerIn: parent
        }
        // 新增按钮
        Rectangle {
            id: add
            width: 30; height: 30
            radius: 30
            border.color: '#4050ff'
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: minus.left
            anchors.rightMargin: 70
            Image {
                anchors.fill: parent
                anchors.margins: 5
                source: './asset/image/plus-solid.svg'
                fillMode: Image.PreserveAspectFit
            }
            MouseArea {
                property bool entered: false
                anchors.fill: parent
                hoverEnabled: true
                onEntered: entered = true
                onExited: entered = false
                onClicked: fileDialog.open()
                ToolTip {
                    visible: parent.entered
                    text: '新增'
                    delay: 500
                }
            }
        }
        // 删除按钮
        Rectangle {
            id: minus
            width: 30; height: 30
            radius: 30
            border.color: '#4050ff'
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 70
            Image {
                anchors.fill: parent
                anchors.margins: 5
                source: './asset/image/trash-solid.svg'
                fillMode: Image.PreserveAspectFit
            }
            MouseArea {
                property bool entered: false
                anchors.fill: parent
                hoverEnabled: true
                onEntered: entered = true
                onExited: entered = false
                ToolTip {
                    visible: parent.entered
                    text: '删除'
                    delay: 500
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        modality: Qt.ApplicationModal
        nameFilters: ["Video files(*.mp4 *.avi)", "All files (*)"]
        selectedNameFilter.index: 0
        onAccepted: {
            console.log("You chose: " + fileDialog.file)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    Loader {
        id: itemLoader
        width: parent.width
        height: parent.height - header.height - footer.height
        anchors.top: parent.top
        anchors.topMargin: header.height
        anchors.bottom: parent.bottom
        anchors.bottomMargin: footer.height
    }
    Component.onCompleted: itemLoader.sourceComponent = videoItem

    Component {
        id: videoItem
        VideoList {
            id: videoList
        }
    }

    Component {
        id: audioItem
        AudioList {
            id: audioList
        }
    }

    // 底部导航栏
    Rectangle {
        id: footer
        width: parent.width
        height: 80
        anchors.bottom: parent.bottom

        Rectangle {
            id: videos
            width: parent.width / 2
            height: parent.height
            border.color: '#409eff'
            Image {
                id: videoIcon
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width / 2 - videosText.width / 2 - 35
                width: 30; height: 22
                source: './asset/image/video-icon.svg'
            }
            Text {
                id: videosText
                anchors.centerIn: parent
                text: '视频'
                font.pixelSize: 20
                color: '#409eff'
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    itemLoader.sourceComponent = videoItem
                    videos.border.color = '#409eff'
                    videosText.color = '#409eff'
                    audios.border.color = '#797979'
                    audiosText.color = '#000000'
                }
            }
        }

        Rectangle {
            id: audios
            width: parent.width / 2
            height: parent.height
            anchors.right: parent.right
            border.color: '#797979'
            Image {
                id: audioIcon
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width / 2 - audiosText.width / 2 - 25
                width: 20; height: 20
                source: './asset/image/audio-icon.svg'
            }
            Text {
                id: audiosText
                anchors.centerIn: parent
                text: '音频'
                font.pixelSize: 20
                color: '#000000'
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    itemLoader.sourceComponent = audioItem
                    videos.border.color = '#797979'
                    videosText.color = '#000000'
                    audios.border.color = '#409eff'
                    audiosText.color = '#409eff'
                }
            }
        }
    }
}
