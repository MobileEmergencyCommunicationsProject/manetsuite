// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "globals.js" as Globals

Rectangle {
    id: mcshare
    width: 360
    height: 360

    /*
      TODO: signals from C++

      newMessage(receiver, text, color): Tell the receiver to
                    append colored text to itself. One receiver
                    is "receiver".  The other is "sender".

      RenameFile(oldName, newName, objectHandle): Ask if it's OK to rename the file.
                    Send the answer back to C++.
                    Append the result to the received files list.

      */

    /*
      TODO: functions to C++

      cancel(objectHandle): stop receiving or sending a file.

      startReceiver()
      stopReceiver()
      startSender()
      stopSender()

      write(): write some text into the device.

      */

    MultiSelectionFileDialog {
        id: fileSelector
        visible: false

        // Extra actions to execute when the
        // Accept button is clicked
        //
        function acceptButtonFunction() {
            mcshare.state = ""
        }

        // Extra actions to execute when the
        // Cancel button is clicked
        //
        function cancelButtonFunction() {
            mcshare.state = ""
        }

    }

    ReceiveFiles {
        id:receiveFiles
        buttonColor: "light blue"
        startButtonText: qsTr("Start Receiver")
        stopButtonText: qsTr("Stop Receiver")
        title: qsTr("Receive Files")
        visible: false

        // Extra actions to execute when the
        // button is clicked
        //
        function backButtonClickedFunction() {
            console.debug("Back")
            mcshare.state = ""
        }

        function startButtonClickedFunction() {
            console.debug("Start Receiver")
        }

        function stopButtonClickedFunction() {
            console.debug("Stop Receiver")
        }
    }

    ReceiveFiles {
        id:sendFiles
        buttonColor: "light blue"
        startButtonText: qsTr("Start Sender")
        stopButtonText: qsTr("Stop Sender")
        title: qsTr("Send Files")
        visible: false

        // Extra actions to execute when the
        // button is clicked
        //
        function backButtonClickedFunction() {
            console.debug("Back")
            mcshare.state = ""
        }

        function startButtonClickedFunction() {
            console.debug("Start Sender")
        }

        function stopButtonClickedFunction() {
            console.debug("Stop Sender")
        }    }

    Column {
        id: column1
        property int borderWidth: 2
        property color buttonColor: "light blue"
        anchors.fill: parent
        anchors.margins: 2
        spacing: 6

        Rectangle {
            id: chooseButton
            border.width: column1.borderWidth
            color: column1.buttonColor
            height: parent.height / 3
            width: parent.width

            Text {
                id: text1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Choose")
                font.pixelSize: 12
            }

            MouseArea {
//                id: chooseButtonMouseArea
                anchors.fill: parent
                onClicked: {
                    mcshare.state = "chooseState"
                    console.debug("choose")
                }
            }
        }

        Rectangle {
            id: receiveButton
            border.width: column1.borderWidth
            color: column1.buttonColor
            height: parent.height / 3
            width: parent.width

            Text {
                id: text3
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Receive")
                font.pixelSize: 12
            }

            MouseArea {
//                id: receiveButtonMouseArea
                anchors.fill: parent
                onClicked: {
                    mcshare.state = "receiveState"
                    console.debug("receive")
                }
            }
        }

        Rectangle {
            id: sendButton
            border.width: column1.borderWidth
            color: column1.buttonColor
            height: parent.height / 3
            width: parent.width

            Text {
                id: text2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Send")
                font.pixelSize: 12
            }

            MouseArea {
//                id: sendButtonMouseArea
                anchors.fill: parent
                onClicked: {
                    mcshare.state = "sendState"
                    console.debug("send")
                }
            }
        }
    }

    states: [
        State {
            name: "chooseState"
            PropertyChanges {
                target: fileSelector
                visible: true
            }

            PropertyChanges {
                target: column1
                visible: false
            }
        },
        State {
            name: "receiveState"
            PropertyChanges {
                target: receiveFiles
                visible: true
            }

            PropertyChanges {
                target: column1
                visible: false
            }
        },
        State {
            name: "sendState"
            PropertyChanges {
                target: sendFiles
                visible: true
            }

            PropertyChanges {
                target: column1
                visible: false
            }
        }
    ]


}
