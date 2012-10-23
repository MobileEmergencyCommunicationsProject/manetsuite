import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    tools: commonTools
    property string title: ""
    property alias url: helpViewer.url

    function update() {
        console.log(qsTr("Toolbar Update button clicked"))
        helpViewer.reload()
    }

    Component.onCompleted: {
        console.debug("HelpPage: height", height, "width", width)
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
            font.bold: true
            font.family: "Nokia Pure Text"
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter
            text: title
        }
    }

    HelpViewer {
        id: helpViewer
        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom
        clip: true
        width: parent.width
    }
}
