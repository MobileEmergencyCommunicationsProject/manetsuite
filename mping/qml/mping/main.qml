/*
  mping is for testing QNormTransportStream.

  It opens with a screen that has two buttons, SEND and RECEIVE.
  Pressing the SEND button transitions to the send screen.

  The send screen contains a button, STOP, and a display.
  The app sends a message every second until the STOP button is pressed.
  The message contains a number. The display shows the value of the number.
  The number increases by one each time the app sends a message.
  Pressing the STOP button ends transmission, kills the transport, and
  transitions to the main screen.

  Pressing the RECEIVE button transitions to the receive screen.

  The receive screen contains a button, STOP, and a display.
  Whenever the app receives a message, it displays the contents
  in the display.
  Pressing the STOP button ends reception, kills the transport, and
  transitions to the main screen.

  In this version, the mcast address and port are fixed.
  */
// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import NormTest 1.0

Rectangle {
    id: appWindow
    width: 360
    height: 360

    NormTest {
        id: normTest

        // Print the message in the scrollable message window.
        // Ensure that the new messsage is visible.
        onNewMessage: {

            messageListModel.append({"message":message, "messageColor":color});
            messageListView.positionViewAtIndex(messageListView.count - 1, ListView.Contain)
        }
    }

    Column {
        id: column1
        spacing: 10
        anchors.fill: parent

        Rectangle {
            id: sendButton
            height: 50
            color: "green"
            radius: 3
            border.width: 3
            border.color: "#000000"
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            Text {
                id: sendButtonText
                text: qsTr("SEND")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: sendButtonMouseArea
                anchors.fill: parent
                onPressed: {
                    appWindow.state = "SendState"
                    normTest.startSender();
//                    messageListModel.append({"message":"Sender started"});
                }
            }
        }

        Rectangle {
            id: receiveButton
            height: 50
            color: "green"
            radius: 3
            border.width: 3
            border.color: "#000000"
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            Text {
                id: receiveButtonText
                text: qsTr("RECEIVE")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: receiveButtonMouseArea
                anchors.fill: parent
                onPressed: {
                    appWindow.state = "ReceiveState"
                    normTest.startReceiver();
//                    messageListModel.append({"message":"Receiver started"});
                }
            }
        }

        Rectangle {
            id: stopButton
            height: 50
            color: "white"
            border.width: 3
            border.color: "#000000"
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            visible: true

            Text {
                id: stopButtonText
                text: qsTr("STOP")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: stopButtonMouseArea
                visible: true
                anchors.fill: parent
                enabled: false
            }
        }

        Rectangle {
            id: messageArea
            visible: true
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: stopButton.bottom
            anchors.topMargin: 10
            border.color: "blue"
            border.width: 5

            // TODO: Do not show Items that scroll outside of the visible area
            ListView {
                id: messageListView
                anchors.fill: parent
                clip: true
                highlightFollowsCurrentItem: true
                spacing: 5

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
                            text: message
                            color: messageColor
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                model: ListModel {
                    id: messageListModel
                }
            }
        }
    }

    // In one of the running states (ReceiveState, SendState),
    // turn off the Send and Receive buttons and their mouse
    // areas.  Turn on the Stop button and its mouse area.
    states: [
        State {
            name: "SendState"
            PropertyChanges {
                target: receiveButton;
                color: "white"
            }

            PropertyChanges {
                target: receiveButtonMouseArea;
                enabled:false
            }

            PropertyChanges {
                target: sendButtonMouseArea;
                enabled: false
            }

            PropertyChanges {
                target: stopButton;
                color: "red"
            }

            PropertyChanges {
                target: stopButtonMouseArea;
                enabled: true
                onPressed: {
                    normTest.stop();
//                    messageListModel.append({"message":"Sender stopped"});
                    appWindow.state = ""
                }
            }
        },
        State {
            name: "ReceiveState"
            PropertyChanges {
                target: sendButton;
                color: "white"
            }

            PropertyChanges {
                target: sendButtonMouseArea;
                enabled: false
            }

            PropertyChanges {
                target: receiveButtonMouseArea;
                enabled: false
            }

            PropertyChanges {
                target: stopButton;
                color: "red"
            }

            PropertyChanges {
                target: stopButtonMouseArea;
                enabled: true
                onPressed: {
                    normTest.stop();
//                    messageListModel.append({"message":"Receiver stopped"});
                    appWindow.state = ""
                }
            }
        }
    ]
}
