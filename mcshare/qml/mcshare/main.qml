// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "globals.js" as Globals

Rectangle {
    id: rectangle1
    width: 360
    height: 360

    // fileChooser shows all of the files in a directory
    // hierarchy.  Select a file to add it to the list
    // of selected files displayed in chosenFiles, below.
    FileChooser {
        id: fileChooser
        anchors.top: parent.top
        anchors.left:parent.left
        anchors.right:parent.right
        height: (parent.height - buttonRow.height) / 2
        flickableDirection: Flickable.AutoFlickDirection
        folder: documentsPath
        nameFilters: ["*"]
        title: qsTr("Choose files to share")
        onSelectedFileChanged: {
            if (-1 == Globals.chosenFiles.indexOf(selectedFile)) {
                list_view1.model.append({"fileName":selectedFile})
                list_view1.currentIndex = list_view1.count - 1
                Globals.chosenFiles.push(selectedFile)
            }
        }
    }

    // chosenFiles holds the names of the files that
    // the user selected in fileChooser, above.
    // Select an entry in the chosenFiles to remove
    // it.
    Rectangle {
        id: chosenFiles
        anchors.top: fileChooser.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: (parent.height - buttonRow.height) / 2

        function clear() {
            list_view1.model.clear()
        }

        Rectangle {
            id: titleBar
            anchors.top: parent.top
            color: "grey"
            height: 66
            width: parent.width

            Text {
                id: titleBarText
                anchors.centerIn: parent
                text: qsTr("Chosen Files")
                font.bold: true
                font.pixelSize: 26
                font.family: "Nokia Pure Text"
                horizontalAlignment: Text.AlignHCenter
            }
        }

        ListView {
            id: list_view1
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: titleBar.bottom
            clip: true
            delegate: fileDelegate
            highlightFollowsCurrentItem: true
            interactive: true
            model: listModel

            Component {
                id: fileDelegate

                Rectangle {
                    id: wrapper
                    border.color:  "black"
                    border.width: ListView.isCurrentItem ? 2 : 0
                    color: ListView.isCurrentItem ? "grey" : "white"
                    height: entry.height + 15
                    radius: 10
                    width: parent.width

                    Text {
                        id: entry
                        anchors.verticalCenter: wrapper.verticalCenter
                        font.family: "Nokia Pure Text"
                        font.pixelSize: 24
                        text: fileName
                        color: wrapper.ListView.isCurrentItem ? "white" : "black"
                    }

                    MouseArea {
                        anchors.fill: wrapper
                        onClicked: {
                            parent.ListView.view.currentIndex = index

                            var idx = Globals.chosenFiles.indexOf(fileName)
                            Globals.chosenFiles.splice(idx, 1)
                            list_view1.model.remove(index)
                            list_view1.currentIndex = list_view1.count - 1
                        }
                    }
                }
            }

            ListModel {
                id:listModel
            }
        }
    }

    Row {
        id: buttonRow
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: 50
        spacing: 5

        Rectangle {
            id: acceptButton
            width: parent.width / 2
            height: parent.height
            color: "green"

            Text {
                id: text1
                text: qsTr("Accept")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: acceptButtonMouseArea
                anchors.fill: acceptButton
                onClicked: {
                    // print the contents of the chosenFiles list model.
                    for (var i=0; i < Globals.chosenFiles.length; i++) {
                        console.debug(Globals.chosenFiles[i])
                    }
                }
            }
        }

        Rectangle {
            id: cancelButton
            width: parent.width / 2
            height: parent.height
            color: "red"

            Text {
                id: text2
                text: qsTr("Cancel")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: cancelButtonMouseArea
                anchors.fill: cancelButton
                onClicked: {
                    Globals.chosenFiles.length = 0
                    chosenFiles.clear()
                }
            }
        }
    }
}
