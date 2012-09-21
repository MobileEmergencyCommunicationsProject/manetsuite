// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "globals.js" as Globals

Rectangle {
    id: container
    anchors.fill: parent

    function buttonClickedFunction () {
        console.debug("CLICKY")
    }

    onVisibleChanged: {
        if (visible) {
            console.debug("Chosen files:")
            for(var i=0; i < Globals.chosenFiles.length; i++) {
                console.debug(Globals.chosenFiles[i])
            }
            console.debug("--------------------")
        }
    }

    Text {
        id: text1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("Send Files")
        font.pixelSize: 12
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            buttonClickedFunction()
        }
    }
}
