import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: container
    tools: commonTools
    property string title: ""
    property alias delegate: list_view1.delegate
    property alias model: list_view1.model
    property alias button: startStopButton

    function buttonClickedFunction() {
        console.log(qsTr("button clicked"))
    }


    Rectangle {
        id: titleBar
        anchors.top: parent.top
        color: "grey"
        height: 66
        width: parent.width

//        Text {
//            id: titleBarText
//            anchors.centerIn: parent
//            font.bold: true
//            font.family: "Nokia Pure Text"
//            font.pixelSize: 26
//            horizontalAlignment: Text.AlignHCenter
//            text: title
//        }
        Label {
            id: label
            anchors.centerIn: parent
            text: container.title
            visible: true
        }
    }

    Rectangle {
        id: listView
        anchors.top: titleBar.bottom
        anchors.bottom: startStopButton.top
        width: parent.width
        radius: 2
        border.width: 2
        border.color: "#000000"

        ListView {
            id: list_view1
            anchors.rightMargin: 2
            anchors.leftMargin: 2
            anchors.bottomMargin: 2
            anchors.topMargin: 2
            anchors.fill: parent
            clip: true
            flickableDirection: Flickable.VerticalFlick
            highlightFollowsCurrentItem: true
            model: ListModel {}
            snapMode: ListView.SnapToItem
            spacing: 5

            ListView.onAdd: {
                positionViewAtIndex(count - 1, ListView.Contain)
            }
        }
    }

    Button{
        id: startStopButton
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 10
        }
        height: 66
        text: qsTr("Foo")
        onClicked: buttonClickedFunction()
    }
}
