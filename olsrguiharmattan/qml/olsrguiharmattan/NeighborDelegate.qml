// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

// NeighborDelegate renders one neighbor in a ListView
Rectangle {
    id: container
    anchors.left: parent.left
    anchors.right: parent.right
    border.color: "red"
    border.width: 2
    height: 130
    width: ListView.view.width

    Column {

        Label {
            id: addressLabel
            text: qsTr("Address: ") + address
        }

        Label {
            id: statusLabel
            text: qsTr("Status: ") + status
        }

        Label {
            id: hysteresisLabel
            text: qsTr("Hysteresis: ") + hysteresis
        }

        Label {
            id: mPRSelectLabel
            text: qsTr("mPR Select: ") + mPRSelect
        }
    }
}
