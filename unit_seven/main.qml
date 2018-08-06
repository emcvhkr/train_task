import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 100
    height: 135
    title: qsTr("Hello World")

    Column {
        anchors.centerIn: parent
        spacing: 2

        Repeater {
            model: ["Enterprise","Colombia","Challenger","Discovery","Endeavour","Atlantis"]
            anchors.centerIn: parent

            Rectangle {
                width: 100
                height: 20
                radius: 3
                color: "lightblue"

                Text {
                    anchors.centerIn: parent
                    text: modelData+"("+index+")"
                }
            }
        }
    }

}
