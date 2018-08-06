import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 512
    height: 320
    title: qsTr("Hello World")

    Image {
        id: root
        source: "images/res/background.png"

        Image {
            id: pole
            //anchors.centerIn: parent
            x:(parent.width-width)/2;y:160
            source: "images/res/pole.png"
        }

        Image {
            id:wheel
            anchors.centerIn: parent

            source: "images/res/pinwheel.png"
            Behavior on rotation {
                NumberAnimation {
                    duration: 250
                }
            }
        }


        MouseArea {
            anchors.fill: parent
            onClicked: wheel.rotation += 90
        }
    }
}

