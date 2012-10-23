// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: container
    tools: commonTools
    property string title: ""

    property bool allLinks : false
    property bool fuzzyFlooding: false
    property bool tcSlowdown: false
    property double helloInterval: 0.0
    property double helloJitter: 0.0
    property double helloTimeoutFactor: 0.0
    property double tcInterval : 0.0
    property double tcJitter: 0.0
    property double tcTimeoutFactor: 0.0
    property double hnaInterval: 0.0
    property double hnaJitter: 0.0
    property double hnaTimeoutFactor: 0.0
    property double hysteresisUp: 0.0
    property double hysteresisDown: 0.0
    property double hysteresisAlpha: 0.0
    property int willingness: 0

    function okFunction() {
        console.log(qsTr("OK button clicked"))
    }

    function update() {
        console.log(qsTr("Toolbar Update button clicked"))
    }

    Component.onCompleted: restoreValues()

    // Change all values at once
    //
    function setValues(al, fuzzy, slowdown,
                       hi, hj, ht,
                       tci, tcj, tct,
                       hnai, hnaj, hnat,
                       up, down, alpha,
                       willing) {
        allLinks_checkBox.checked = allLinks = al
        fuzzyFlooding_checkBox.checked = fuzzyFlooding = fuzzy
        tcSlowDown_checkBox.checked = tcSlowdown = slowdown
        helloInterval_input.value = helloInterval = hi
        helloJitter_input.value = helloJitter = hj
        helloTimeoutFactor_input.value = helloTimeoutFactor = ht
        tcInterval_input.value = tcInterval = tci
        tcJitter_input.value = tcJitter = tcj
        tcTimeoutFactor_input.value = tcTimeoutFactor = tct
        hnaInterval_input.value = hnaInterval = hnai
        hnaJitter_input.value = hnaJitter = hnaj
        hnaTimeoutFactor_input.value = hnaTimeoutFactor = hnat
        hysteresisUp_input.value = hysteresisUp = up
        hysteresisDown_input.value = hysteresisDown = down
        hysteresisAlpha_input.value = hysteresisAlpha = alpha
        willingness = willing

        container.state = ""
    }

    // Restore the (possibly changed) values to their previous
    // ones, the values given in the last call to setValues().
    //
    function restoreValues() {
        allLinks_checkBox.checked = allLinks
        fuzzyFlooding_checkBox.checked = fuzzyFlooding
        tcSlowDown_checkBox.checked = tcSlowdown
        helloInterval_input.value = helloInterval
        helloJitter_input.value = helloJitter
        helloTimeoutFactor_input.value = helloTimeoutFactor
        tcInterval_input.value = tcInterval
        tcJitter_input.value = tcJitter
        tcTimeoutFactor_input.value = tcTimeoutFactor
        hnaInterval_input.value = hnaInterval
        hnaTimeoutFactor_input.value = hnaTimeoutFactor
        hysteresisUp_input.value = hysteresisUp
        hysteresisDown_input.value = hysteresisDown
        hysteresisAlpha_input.value = hysteresisAlpha

        // Note that we don't change willingness.
        // Although willingness can be changed from outside
        // by using setValues(), SettingsPage cannot change
        // willingness. This is because the original NRL
        // OLSR GUI does not show the value of willingness.

        container.state = ""
    }

    // Save the current, possibly changed, values.
    //
    function storeValues() {
        allLinks = allLinks_checkBox.checked
        fuzzyFlooding = fuzzyFlooding_checkBox.checked
        tcSlowdown = tcSlowDown_checkBox.checked
        helloInterval = helloInterval_input.value
        helloJitter = helloJitter_input.value
        helloTimeoutFactor = helloTimeoutFactor_input.value
        tcInterval = tcInterval_input.value
        tcJitter = tcJitter_input.value
        tcTimeoutFactor = tcTimeoutFactor_input.value
        hnaInterval = hnaInterval_input.value
        hnaTimeoutFactor = hnaTimeoutFactor_input.value
        hysteresisUp = hysteresisUp_input.value
        hysteresisDown = hysteresisDown_input.value
        hysteresisAlpha = hysteresisAlpha_input.value

        container.state = ""
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

    Flickable {
        id: flickArea
        anchors {
            bottom: buttons.top
            left: parent.left
            right: parent.right
            top: titleBar.bottom
        }
        clip: true
        contentHeight: parametersColumn.height
        contentWidth: parametersColumn.width
        flickableDirection: Flickable.VerticalFlick

        // nrlolsrd sends: settings <all links> <fuzzy flooding> <tc slow down> \
        // <hello interval> <hello jitter> <hello timeout factor> \
        // <tc interval> <tc jitter> <tc timeout factor> \
        // <hna interval> <hna jitter> <hna timeout factor> \
        // <hysteresis up> <hysteresis down> <hysteresis alpha> <local willingness>
        //
        // <all links>, <fuzzy flooding>, <tc slow down>, and <local willingness> are ints. They represent bools.
        // The rest are doubles.

        Column {
            id: parametersColumn
            anchors.fill: parent
            spacing: 4

            ButtonRow {
                id: checkboxes
                exclusive: false

                CheckBox {
                    id: allLinks_checkBox
                    text: "all links"

                    onClicked: {
                        container.state = "valuesChanged"
                    }
                }
                CheckBox {
                    id: fuzzyFlooding_checkBox
                    text: "fuzzy"

                    onClicked: {
                        container.state = "valuesChanged"
                    }
                }
                CheckBox {
                    id: tcSlowDown_checkBox
                    text: "slowdown"

                    onClicked: {
                        container.state = "valuesChanged"
                    }
                }
            }


            // HELLO

            DoubleInputForm {
                id: helloInterval_input
                label: "Hello Interval: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id:helloJitter_input
                label: "Hello Jitter: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id: helloTimeoutFactor_input
                label: "Hello Timeout: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            // TC

            DoubleInputForm {
                id: tcInterval_input
                label: "TC Interval: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id: tcJitter_input
                label: "TC Jitter: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id:tcTimeoutFactor_input
                label: "TC Timeout: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            // HNA

            DoubleInputForm {
                id:hnaInterval_input
                label: "HNA Interval: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id: hnaJitter_input
                label: "HNA Jitter: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id: hnaTimeoutFactor_input
                label: "HNA Timeout: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            // Hysteresis

            DoubleInputForm {
                id: hysteresisUp_input
                label: "Hysteresis Up: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id: hysteresisDown_input
                label: "Hysteresis Down: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }

            DoubleInputForm {
                id: hysteresisAlpha_input
                label: "Hysteresis Alpha: "
                value: "0.0"

                onValueChanged: {
                    container.state = "valuesChanged"
                }
            }
        }
    }

    Row {
        id: buttons
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 5

        Button {
            id: okButton
            height: 66
            text: qsTr("OK")
            width: 100
            onClicked: {
                storeValues()
                okFunction()
            }
        }

        Button {
            id: cancelButton
            height: 66
            text: qsTr("Cancel")
            width: 100
            onClicked: {
                restoreValues()
            }
        }
    }

    states: [
        State {
            name: ""
            PropertyChanges {
                target: cancelButton
                enabled: false
            }
            PropertyChanges {
                target: okButton
                enabled: false
            }
        },
        State {
            name: "valuesChanged"
            PropertyChanges {
                target: cancelButton
                enabled: true
            }
            PropertyChanges {
                target: okButton
                enabled: true
            }
        }
    ]
}
