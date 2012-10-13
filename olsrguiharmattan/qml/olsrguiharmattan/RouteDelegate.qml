// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

// RouteDelegate renders one route in a ListView
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
            id: destinationLabel
            text: qsTr("Destination: ") + destination
        }

        Label {
            id: gatewayLabel
            text: qsTr("Gateway: ") + gateway
        }

        Label {
            id: weightLabel
            text: qsTr("Weight: ") + weight
        }

        Label {
            id: interfaceLabel
            text: qsTr("Interface: ") + interfaceName
        }
    }
}
