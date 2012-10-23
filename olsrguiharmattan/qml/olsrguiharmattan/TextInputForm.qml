// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: container
    height: 30
    width: 40

    property alias label: label.text
    property alias maximumLength: input.maximumLength
    property alias validator: input.validator
    property alias value: input.text

    Text {
        id: label

        font.bold: true
        font.family: "Nokia Pure Text"
        font.pixelSize: 26
    }

    DecoratedTextInput {
        id: input
        anchors.left: label.right
        anchors.margins: 2
        smooth: true
    }
}
