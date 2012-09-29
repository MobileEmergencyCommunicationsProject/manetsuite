// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "globals.js" as Globals

/*
  MultiSelectionFileDialog is for selecting an arbitrary number of files from a
  file system. It returns the names of the selected files in
  Globals.chosenFiles.

  A MultiSelectionFileDialog consists of two scrollable windows, one above the
  other. Below them are two buttons: Accept and Clear. The upper window is for
  navigating the file system and choosing a single file. The lower window
  displays the names of the chosen files. To select a file, touch it in the
  upper window. To deselect a file, touch it in the lower window.

  Press the Accept button to close the dialog and return. When the Accept
  button is pressed, it invokes acceptButtonFunction() before returning. An
  instance of MultiSelectionFileDialog may have its own acceptButtonFunction().

  Press the Clear button to discard list of selected files and return. When
  the Clear button is pressed, it invokes clearButtonFunction() before
  returning. An instance of MultiSelectionFileDialog may have its own
  clearButtonFunction().
  */
Rectangle {
    id: container
    width: 360
    height: 360

    function acceptButtonFunction() {
        console.debug("ACCEPT!")
    }

    function clearButtonFunction() {
        console.debug("CLEAR!")
    }

    function clear() {
        Globals.chosenFiles.length = 0
        list_view1.model.clear()
        list_view1.currentIndex = -1
        fileChooser.currentIndex = -1
    }

    function deselect(index) {
        var message = list_view1.model.get(index).fileName
        var idx = Globals.chosenFiles.indexOf(message)
        Globals.chosenFiles.splice(idx, 1)
        list_view1.model.remove(index)
        list_view1.currentIndex = list_view1.count - 1
    }

    function getSelectedFile(index) {
        var answer = ""
        var obj = list_view1.model.get(index)

        if (undefined != obj) {
            answer = obj.fileName
        }

        return answer
    }

    function selectFile(fileName) {
        if (-1 == Globals.chosenFiles.indexOf(fileName)) {
            list_view1.model.append({"fileName":fileName})
            list_view1.currentIndex = list_view1.count - 1
            Globals.chosenFiles.push(fileName)
        }
    }

    //
    // fileChooser shows all of the files in a directory hierarchy. Select a
    // file to add it to the list of selected files displayed in chosenFiles,
    // below.
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
            selectFile(selectedFile)
        }
    }

    //
    // chosenFiles holds the names of the files that the user selected in
    // fileChooser, above. Select an entry in the chosenFiles to remove it.
    //
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
            height: 33 //66
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

            Component.onCompleted: {
                flickableDirection = Flickable.HorizontalAndVerticalFlick
            }

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
                        color: wrapper.ListView.isCurrentItem ? "white" : "black"
                        font.family: "Nokia Pure Text"
                        font.pixelSize: 24
                        text: fileName
                    }

                    MouseArea {
                        anchors.fill: wrapper
                        onClicked: {
                            parent.ListView.view.currentIndex = index
                            deselect(index)
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

                    acceptButtonFunction()
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
                text: qsTr("Clear")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: cancelButtonMouseArea
                anchors.fill: cancelButton
                onClicked: {
                    clear()
                    clearButtonFunction()
                }
            }
        }
    }
}
