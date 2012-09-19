// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import Qt.labs.folderlistmodel 1.0
//import com.nokia.meego 1.0     // MeeGo components

Rectangle {
    id: container
    property alias count: folderModel.count
    property alias flickableDirection: listView.flickableDirection
    property alias folder: folderModel.folder
    property alias nameFilters: folderModel.nameFilters
    property alias parentFolder: folderModel.parentFolder
    property string selectedFile
    property alias showDirs: folderModel.showDirs
    property alias showDotandDotDot: folderModel.showDotAndDotDot
    property alias showOnlyReadable: folderModel.showOnlyReadable
    property alias sortField: folderModel.sortField
    property alias sortReversed: folderModel.sortReversed
    property alias title : titleBarText.text

    Rectangle {
        id: titleBar
        anchors.top: parent.top
        color: "grey"
        height: 66
        width: parent.width

        Text {
            id: titleBarText
            anchors.centerIn: parent
            text: qsTr("Choose a Card Deck")
            font.bold: true
            font.pixelSize: 26
            font.family: "Nokia Pure Text"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    ListView {
        id: listView
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleBar.bottom
        clip: true
        delegate: fileDelegate
        interactive: true
        model: folderModel

        FolderListModel {
             id: folderModel
             folder: "file:///"
             showDirs: true
             showDotAndDotDot: true
             sortField: "Name"
         }

        FileDelegate {
            id: fileDelegate
        }
    }
}
