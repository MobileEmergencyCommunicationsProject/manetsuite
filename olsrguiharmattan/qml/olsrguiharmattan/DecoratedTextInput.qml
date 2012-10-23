// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: container
    border.width: 2
    border.color: "black"
    height: 30
    radius: 5
    smooth: true
    width: 100

    property alias maximumLength: input.maximumLength
    property alias validator: input.validator
    property alias text: input.text

//    function displayTextChangedFunction() {
//        console.log(qsTr("value changed to ") + input.text)
//    }

    TextInput {
        id: input
        font.family: "Nokia Pure Text"
        font.pixelSize: 26
        width: parent.width - font.pixelSize / 2

//        onDisplayTextChanged: {
//            displayTextChangedFunction()
//        }
    }
}
