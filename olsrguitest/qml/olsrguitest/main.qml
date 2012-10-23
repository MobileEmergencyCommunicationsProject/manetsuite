import QtQuick 1.1
import com.nokia.meego 1.0
import OlsrGuiTest 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    MainPage {
        id: mainPage

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

        states: [
            State {
                name: ""
                PropertyChanges {
                    target: mainPage.button
                    text: qsTr("Start")
                }
                StateChangeScript {
                    name: "stoppedScript"
                    script: {
                        test.stop()
                    }
                }
            },
            State {
                name: "running"
                PropertyChanges {
                    target: mainPage.button
                    text: qsTr("Stop")
                }
                StateChangeScript {
                    name: "runningScript"
                    script: {
                        test.start()
                    }
                }
            }
        ]

        title: qsTr("OLSR GUI Test Daemon")

        Component.onCompleted: {
            mainPage.button.text = qsTr("Start")
        }

        function buttonClickedFunction() {
            mainPage.state == "running" ? mainPage.state = "" : mainPage.state = "running"
        }
    }


    OlsrGuiTest {
        id: test

        onNewMessage: {
            messageListModel.append({"message":message, "messageColor":color})
        }
    }


    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
        }
    }
}
