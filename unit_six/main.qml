import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Image {
        id: ufo
        source: "images/res/ufo.png"
        x: 30; y: 350

    }
    MouseArea {
        anchors.fill: parent
        onClicked: ufo_anima.start()
    }

    ParallelAnimation {
        id: ufo_anima
        //running: true
        NumberAnimation {
            target: ufo
            property: "x"
            from: 30
            to: 450
            duration: 4000
        }
        NumberAnimation {
            target: ufo
            property: "y"
            from :350
            to: 20
            duration: 4000
        }
    }

}
