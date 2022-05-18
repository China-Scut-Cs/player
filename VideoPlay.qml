import QtQuick
import QtMultimedia
import QtQuick.Controls

Item {
    id: videoPlayPage
    anchors.fill: parent

    required property AVItem avItem
    property bool fullScreen: false

    function getSeekTime() {
        if(ctrl.sliderPressed) {
            var min = Math.floor((ctrl.value) / 60)
            var sec = Math.floor(ctrl.value) % 60
            console.log(min + ' : ' + sec)
        }
    }

    Connections {
        target: ctrl
        function onValueChanged() { getSeekTime() }
    }

    TitleSection {
        id: til
        opacity: 1
        onClick: {
            console.log('点击返回按钮')
            home.visible = true
            videoPlayPage.destroy()
            showNormal()
        }
        Behavior on opacity {
            NumberAnimation { duration: 800 }
        }
    }

    ControlSection {
        id: ctrl
        opacity: 1
        fullScreen: videoPlayPage.fullScreen
        anchors.bottom: parent.bottom

        onShowMetadata: metadataShow.visible = !metadataShow.visible
        onHoveredVolume: volume.visible = true
        onClickedRate: rate.visible = true
        onPlayClicked: mediaPlayer.play()

        Behavior on opacity {
            NumberAnimation { duration: 800 }
        }
    }

    MediaPlayer {
        id: mediaPlayer
        videoOutput: videoOutput
        audioOutput: AudioOutput {
            id: audio
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

    // 音量控制部分
    Rectangle {
        id: volume
        property real value: 0
        property bool pressed: false
        width: 40; height: 115
        x: fullScreen ? 1191 : 935
        y: fullScreen ? 777 - volume.height : 633 - volume.height
        border.color: '#aaaaaa'
        visible: false
        z: 20

        Label {
            id: volumeTxt
            text: '0'
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 7
        }

        Slider {
            id: volumeSlider
            orientation: Qt.Vertical
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            from: 0; to: 100
            snapMode: "SnapAlways"
            z: 21

            background: Rectangle {
                id: volume1
                width: 8
                height: 90
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 40
                border.color: '#797979'
                Rectangle {
                    id: wolume2
                    width: volume1.width
                    height: volume1.height - volumeSlider.visualPosition * volume1.height
                    anchors.bottom: volume1.bottom
                    color: '#409eff'
                    radius: 7
                }
            }
            handle: Rectangle {
                x: volumeSlider.availableWidth / 2 - implicitWidth / 2
                y: volumeSlider.visualPosition * (volumeSlider.availableHeight - implicitHeight)
                implicitWidth: 15
                implicitHeight: 15
                color: '#409eff'
                radius: 15
            }

            onValueChanged: {
                volumeTxt.text = value.toFixed(0)
            }
            onPressedChanged: {
                hover5SecTimer.stop()
                hoverArea.enabled = false
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                hover5SecTimer.stop()
                hoverArea.enabled = false
            }
            onExited: {
                hover5SecTimer.start()
                hoverArea.enabled = true
                volume.visible = false
            }
        }
    }

    // 倍速控制部分
    Rectangle {
        id: rate
        width: 50; height: 116
        x: fullScreen ? 1191 + 65 : 935 + 65
        y: fullScreen ? 777 - rate.height : 633 - rate.height
        color: '#aaaaaa'
        z: 20
        visible: false
        Column {
            id: rateChoice
            anchors.fill: parent
            anchors.top: parent.top
            anchors.topMargin: 7
            spacing: 14
            z: 20
            Button {
                id: rate_8
                width: parent.width; height: 16
                flat: true
                text: ' ×8.0 '
                onClicked: console.log('点击8倍速按钮')
            }
            Button {
                id: rate_4
                width: parent.width; height: 16
                flat: true
                text: ' ×4.0 '
                onClicked: console.log('点击4倍速按钮')
            }
            Button {
                id: rate_2
                width: parent.width; height: 16
                flat: true
                text: ' ×2.0 '
                onClicked: console.log('点击2倍速按钮')
            }
            Button {
                id: rate_1
                width: parent.width; height: 16
                flat: true
                text: ' ×1.0 '
                onClicked: console.log('点击1倍速按钮')
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onEntered: {
                hover5SecTimer.stop()
                hoverArea.enabled = false
            }
            onExited: {
                hover5SecTimer.start()
                hoverArea.enabled = true
                rate.visible = false
            }
        }
    }

    // 详细信息展示部分
    Rectangle {
        id: metadataShow
        width: 150
        height: fullScreen ? 864 : 720
        anchors.right: parent.right
        color: 'red'
        visible: false
        Text {
            id: metadataInfo
            anchors.centerIn: parent
            text: avItem.title + ' ' + avItem.author
        }
    }

    // 标题栏与进度条隐藏计时器
    Timer {
        id: hover5SecTimer
        interval: 5000
        repeat: false
        running: false
        triggeredOnStart: false
        onTriggered: {
            til.opacity = 0
            ctrl.opacity = 0
        }
    }
    MouseArea {
        id: hoverArea
        width: parent.width
        height: parent.height
        hoverEnabled: true
        onEntered: {
            til.opacity = 1
            ctrl.opacity = 1
            hover5SecTimer.start()
        }
        onPositionChanged: {
            til.opacity = 1
            ctrl.opacity = 1
            hover5SecTimer.restart()
        }
        onExited: {
            til.opacity = 0
            ctrl.opacity = 0
            hover5SecTimer.stop()
        }
        onDoubleClicked: {
            fullScreen ?  showNormal() : showFullScreen()
            fullScreen = !fullScreen
        }
    }
}
