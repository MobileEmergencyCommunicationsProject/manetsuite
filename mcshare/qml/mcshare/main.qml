// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: rectangle1
    width: 360
    height: 360

    FileChooser {
        id: fileChooser
        anchors.top: parent.top
        anchors.left:parent.left
        anchors.right:parent.right
        height: parent.height / 2
        flickableDirection: Flickable.AutoFlickDirection
        folder: documentsPath
        nameFilters: ["*"]
        title: qsTr("Choose files to share")
        onSelectedFileChanged: {
            list_view1.model.append({"fileName":selectedFile})
            list_view1.currentIndex = list_view1.count - 1
            // TODO: don't append() if selectedFile
            // is alreay in list_view1.model.
            // HOW DO I DO THAT?
            //
            // Consider putting two FileChoosers
            // on display.  The first one has ["*"]
            // as the nameFilters.  It is used to
            // choose files. The second one adjusts its
            // nameFilters to display only the names of
            // the chosen files. E.g. If the user
            // hasn't selected any files, then the second
            // FileChooser is empty. If the user selects
            // file "A", then the second FileChooser displays
            // "A".  If the user selects "B", then the second
            // FileChooser displays "A" and "B".
            //
            // Selecting a file in the second FileChooser
            // removes it from the set of selected files.
            //
        }
    }

    Rectangle {
        id: chosenFiles
        anchors.top: fileChooser.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

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

            FileDelegate {
                id:fileDelegate
            }

            ListModel {
                id:listModel
            }
        }
    }
}
