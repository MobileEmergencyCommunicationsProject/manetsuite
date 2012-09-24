// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import "globals.js" as Globals
import NormFileTest 1.0
import QtQuick 1.1

Rectangle {
    id: mcshare
    width: 360
    height: 360

    NormFileTest {
        id: app

        //
        // The app is ready for us to send files.
        //
        onReadyWrite: {
            fileSelector.sendFiles()
        }
    }

    /*
      Display a yes-no dialog.
      All arguments are strings.

      Return true if the user presses yes.
      Return false if the user presses no.
      */
    function dialog(message, yes_button, no_button) {
        console.debug(message)
        console.debug("yes=", yes_button)
        console.debug("no=", no_button)
        return true
    }

    /*
      Append a message to the message are of the
      receiveFiles or sendFiles page.
      */
    function addMessage(destination, message) {
        if ("receiver" == destination) {
            receiverPage.append(message)
        } else if ("sender" == destination) {
            senderPage.append(message)
        } else {
            console.log("addMessage(): destination must be \"receiver\" or \"sender\", not",
                        destination)
        }
    }

    /*
      Dialog for choosing files to send via NORM. When the user presses the
      Accept button, give the name of each selected file to the app one at a
      time until the app knows all file names or the app can't accept any more
      file names.

      This dialog forgets the names of all unsent file when the user presses
      the Clear button.
      */
    MultiSelectionFileDialog {
        id: fileSelector
        visible: false

        //
        // Extra actions to execute when the Accept button is clicked
        //
        function acceptButtonFunction() {
            mcshare.state = ""
            sendFiles()
        }

        //
        // Extra actions to execute when the Clear button is clicked
        //
        function clearButtonFunction() {
            mcshare.state = ""
        }

        function sendFiles() {
            for(var i in Globals.chosenFiles) {
                if (app.sendFile(Globals.chosenFiles[i])) {
                    deselect(Globals.chosenFiles[i])
                } else {
                    break
                }
            }
        }
    }

    ReceiveFiles {
        id:receiverPage
        buttonColor: "light blue"
//        startButtonText: qsTr("Start Receiver")
//        stopButtonText: qsTr("Stop Receiver")
        title: qsTr("Receive Files")
        visible: false

        // Extra actions to execute when the
        // button is clicked
        //
        function backButtonClickedFunction() {
            console.debug("Back")
            mcshare.state = ""
        }

//        function startButtonClickedFunction() {
//            console.debug("Start Receiver")
//        }

//        function stopButtonClickedFunction() {
//            console.debug("Stop Receiver")
//        }
    }

    ReceiveFiles {
        id:senderPage
        buttonColor: "light blue"
//        startButtonText: qsTr("Start Sender")
//        stopButtonText: qsTr("Stop Sender")
        title: qsTr("Send Files")
        visible: false

        // Extra actions to execute when the
        // button is clicked
        //
        function backButtonClickedFunction() {
            console.debug("Back")
            mcshare.state = ""
        }

//        function startButtonClickedFunction() {
//            console.debug("Start Sender")
//        }

//        function stopButtonClickedFunction() {
//            console.debug("Stop Sender")
//        }
    }

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
                target: receiverPage
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
                target: senderPage
                visible: true
            }

            PropertyChanges {
                target: column1
                visible: false
            }
        }
    ]


}
