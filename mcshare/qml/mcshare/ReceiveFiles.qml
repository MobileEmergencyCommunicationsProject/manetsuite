// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "globals.js" as Globals

Rectangle {
    id: container   
    property alias backButtonText: backButtonText.text
    property color buttonColor: "white"
//    property alias startButtonText: startButtonText.text
//    property alias stopButtonText: stopButtonText.text
    property alias title: titleBarText.text

    anchors.fill: parent
    width: 360
    height: 360

    function append(message) {
        messageListModel.append({"message":message})
    }

    function backButtonClickedFunction () {
//        console.debug("BACK")
    }

//    function startButtonClickedFunction () {
//        console.debug("START")
//    }

//    function stopButtonClickedFunction () {
//        console.debug("STOP")
//    }

    Rectangle {
        id: titleBar
        anchors.top: parent.top
        color: "grey"
        height: 33
        width: parent.width

        Text {
            id: titleBarText
            anchors.centerIn: parent
            text: qsTr("Control")
            font.bold: true
            font.pixelSize: 26
            font.family: "Nokia Pure Text"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Rectangle {
        id: messageArea
        visible: true
        anchors.bottom: buttonRow.top
        anchors.top: titleBar.bottom
        anchors.topMargin: 10
        border.width: 2
        width: parent.width

        ListView {
            id: messageListView
            anchors.fill: parent
            clip: true
            highlightFollowsCurrentItem: true
            spacing: 4

            ListView.onAdd: {
                positionViewAtIndex(count - 1, ListView.Contain)
            }

            delegate: Item {
                width: parent.width
                height: 50
                Rectangle {
                    anchors.bottomMargin: 1
                    anchors.fill: parent
                    anchors.leftMargin:5
                    anchors.rightMargin:5
                    border.width: 2
                    border.color: "yellow"

                    Text {
//                        color: messageColor
                        anchors.verticalCenter: parent.verticalCenter
                        text: message
                    }
                }
            }

            model: ListModel {
                id: messageListModel
            }
        }
    }

    Row {
        id: buttonRow
        property int borderWidth: 2

        anchors.bottom: parent.bottom
        height: 50
        spacing: 6
        width: parent.width

//        Rectangle {
//            id: startButton
//            border.width: buttonRow.borderWidth
//            color: buttonColor
//            height: parent.height
//            width: parent.width / 3

//            Text {
//                id: startButtonText
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Start")
//                font.pixelSize: 12
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    startButtonClickedFunction()
//                }
//            }
//        }

//        Rectangle {
//            id: stopButton
//            border.width: buttonRow.borderWidth
//            color: buttonColor
//            height: parent.height
//            width: parent.width / 3

//            Text {
//                id: stopButtonText
//                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.verticalCenter: parent.verticalCenter
//                text: qsTr("Stop")
//                font.pixelSize: 12
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    stopButtonClickedFunction()
//                }
//            }
//        }

        Rectangle {
            id: backButton
            border.width: buttonRow.borderWidth
            color: buttonColor
            height: parent.height
//            width: parent.width / 3
            width: parent.width // Change this if you change the number of buttons

            Text {
                id: backButtonText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Back")
                font.pixelSize: 12
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    backButtonClickedFunction()
                }
            }
        }
    }
}
