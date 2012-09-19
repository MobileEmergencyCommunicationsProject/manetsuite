// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

// FileDelegate is intended for use as the
// delegate of a FolderListModel.
//
// This delegate forces the FolderListModel
// to walk a directory tree.  See the onClicked
// part of the MouseArea.
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

                // Is this cheating?
                // Ascend or descend into a directory
                // if the user clicks on one.
                if (parent.ListView.view.model.isFolder(index)) {
                    parent.ListView.view.model.folder = filePath
                }
                else {
                    // This has to be defined by the container
                    selectedFile = filePath
                }
            }
        }
    }
}
