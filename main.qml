import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls

Window {
    id: mainWin
    width: 1280
    height: 720
    visible: true
    title: qsTr("桌面端本地播放器")

    Loader {
        id: loader
        anchors.centerIn: parent
    }

    Component.onCompleted: loader.sourceComponent = home

    Component {
        id: home
        Home {
            width: mainWin.width
            height: mainWin.height
            anchors.centerIn: parent
        }
    }
}
