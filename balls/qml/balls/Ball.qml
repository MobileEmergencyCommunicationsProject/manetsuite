// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

// Make a ball to bounce
Rectangle {
    id: ball

    // Add a property for the target y coordinate
    property variant direction : "right"
    property int duration : 2000
    property string uniqueId
    property int velocity : 200
    x: 20; width: 20; height: 20; z: 1
    color: "Lime"

    signal clicked(string uuid)

    // Move the ball to the right and back to the left repeatedly
    SequentialAnimation on x {
        id: animation
        loops: Animation.Infinite
        NumberAnimation { to: parent.width - 40; duration: ball.duration }
        PropertyAction { target: ball; property: "direction"; value: "left" }
        NumberAnimation { to: 20; duration: ball.duration }
        PropertyAction { target: ball; property: "direction"; value: "right" }
    }

    // Make y move with a velocity of 200
    Behavior on y { SpringAnimation{ velocity: ball.velocity; }
    }

    Component.onCompleted: y = parent.height-10; // start the ball motion

    // Detect the ball hitting the top or bottom of the view and bounce it
    onYChanged: {
        if (y <= 0) {
            y = parent.height - 20;
        } else if (y >= parent.height - 20) {
            y = 0;
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onPressed: {
            console.debug("clicked ", uniqueId)
            parent.clicked(uniqueId)
        }
    }
}
