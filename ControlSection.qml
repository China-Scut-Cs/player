import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    property bool containMouseing: false
    property bool play: false
    property string playImage: "./asset/image/play-solid.svg"
    property string pauseImage: "./asset/image/pause-solid.svg"
    property string currentTime: '00:00'
    property string totalTime: '00:00'
    property bool sliderPressed: false
    property bool fullScreen: false
    property int seek: 0
    property real value: 0

    signal playClicked
    signal pauseClicked
    signal previousClicked
    signal nextClicked
    signal hoveredVolume
    signal clickedRate
    signal showMetadata

    width: parent.width
    height: 87
    color: '#aaaaaa'
    z: 90

    Slider {
        id: slider
        x: 0
        y: 15
        height: 15
        width: root.width
        from: 0
        snapMode: "SnapAlways"
        value: root.seek
        to: 999
        z: 5

        background: Rectangle {
            id: sliderBgd
            width: slider.availableWidth
            height: 10
            color: '#ffffff'
            Rectangle {
                id: playedBgd
                width: slider.visualPosition * sliderBgd.width
                height: sliderBgd.height
                color: '#409eff'
            }
        }
        handle: Rectangle {
            x: slider.visualPosition * (slider.availableWidth - implicitWidth)
            y: slider.availableHeight / 2 - implicitHeight / 2
            implicitWidth: 10
            implicitHeight: 25
            radius: 5
            color: '#409eff'
        }

        onValueChanged: {
            root.value = value
        }
        onPressedChanged: {
            root.sliderPressed = pressed
            hover5SecTimer.stop()
            hoverArea.enabled = false
        }
    }

    // 上一个
    Button {
        id: previousBtn
        x: 80; y: 39
        width: 38; height: 48
        flat: true
        z: 5
        icon.source: './asset/image//backward-step-solid.svg'
        icon.width: 38
        icon.height: 48
        onClicked: {
            root.previousClicked()
            console.log('点击上一个按钮')
        }
    }

    // 播放/暂停
    Button {
        id: playBtn
        x: 143
        y: 37
        height: 50
        width: 50
        flat: true
        icon.source : playImage
        icon.width: 20
        icon.height: 26
        z: 5

        background: Rectangle {
           radius: 25
           opacity: enabled ? 1 : 0.3
           color: playBtn.down ? "#d0d0d0" : "#e0e0e0"
       }

        onClicked: {
            root.play = !root.play
            if(!root.play) {
                icon.source = './asset/image/play-solid.svg'
                root.playClicked()
            } else {
                icon.source = './asset/image/pause-solid.svg'
                root.pauseClicked()
            }
            console.log('播放按钮点击 + ' + root.play)
        }
    }

    // 下一个
    Button {
        id: nextBtn
        x: 218; y: 39
        width: 38; height: 48
        flat: true
        icon.source: './asset/image/forward-step-solid.svg'
        icon.width: 38
        icon.height: 48
        z: 5
        onClicked: {
            root.nextClicked()
            console.log('点击下一个按钮')
        }
    }

    Text {
        id: timeDisplay
        anchors.horizontalCenter: parent.horizontalCenter
        y: 40
        font.pointSize: 18
        text: root.currentTime + ' / ' + root.totalTime
        z: 5
    }

    Row {
        id: controlBtns
        y: 32
        width: 345
        anchors.right: parent.right
        spacing: 30
        z: 5

        // 音量控制
        Button {
            id: volumeBtn
            property bool isSlient: false
            width: 40; height: 46
            flat: true
            icon.source: './asset/image/volume-high.svg'
            icon.width: 38; icon.height: 48
            hoverEnabled: true
            onHoveredChanged: {
                root.hoveredVolume()
            }
            onClicked: {
                // 静音
                isSlient = !isSlient
            }
        }

        // 倍速控制
        Text {
            id: rateControl
            width: contentWidth; height: 55
            y: 8
            text: '倍速'
            font.pixelSize: 20
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.clickedRate()
                }
            }
        }

        // 详细信息
        Text {
            id: metadata
            width: contentWidth; height: 55
            y: 8
            text: '详细信息'
            font.pixelSize: 20
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.showMetadata()
                }
            }
        }

        // 全屏按钮
        Button {
            id: fullScreenBtn
            width: 40; height: 46
            flat: true
            icon.source: './asset/image/expand-solid.svg'
            icon.width: 38; icon.height: 38
            onClicked: {
                fullScreen ? showNormal() : showFullScreen()
                fullScreen = !fullScreen
                videoPlayPage.fullScreen = fullScreen
                console.log(root.y + root.height)
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            hover5SecTimer.stop()
            hoverArea.enabled = false
            root.visible = true
        }
        onExited: {
            hover5SecTimer.start()
            hoverArea.enabled = true
        }
    }
}
