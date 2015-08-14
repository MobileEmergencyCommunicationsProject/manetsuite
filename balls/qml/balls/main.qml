/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

// This is an animation playground based on the tvtennis example
// included in Nokia's QtSDK.
//
// Removed: game paddles
// Removed: game board divider
// Removed: score
//
// Added: touch the screen to create a ball at the point of touch
// Added: touch a ball to delete it
//
// TODO:
// 0. Join a multicast group
// 1. Array of balls indexed by QUuid
// 2. When a ball is created
//    a. give it a QUuid
//    b. announce its creation including QUuid, duration, velocity, x, y, z to the multicast group
// 3. When a ball is destroyed
//    a. Remove it from the array of balls
//    b. Announce its destruction including QUuid to the multicast group
//
// 4. When a ball creation announcement arrives,
//    a. Create the ball with the QUuid, duration, velocity, x, y, z
//    b. Add it to the array of balls
// 5. When a ball destruction announcement arrives
//    a. Destroy the ball with the QUuid
//    b. Remove it from the array of balls
//
// Use signals and slots to deal with ball creation and destruction events.
//

// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import AppLogic 1.0
import "objectstore.js" as ObjectStore

Rectangle {
    id: page
    width: 640; height: 480
    color: "Black"

    property Component itemComponent : Qt.createComponent("Ball.qml")
    signal add(string uuid, int duration, int velocity, real x, real y, real z)
    signal remove(string uuid)

    Component.onCompleted: {
        console.debug("PlayerID:", appLogic.playerID)
    }

    function removeBall(uuid) {
        console.debug("Emitting signal REMOVE ", uuid)
        appLogic.on_remove(uuid)

        var ball = ObjectStore.remove(uuid)
        if (null != ball) ball.destroy()
    }

    AppLogic {
        id: appLogic

        onAdd: {
            console.debug("Received signal ADD ", uuid, duration, velocity, x, y, z)
            var ball = itemComponent.createObject(page,
                                                  {"color":color, "duration": duration, "uniqueId":uuid,
                                                      "velocity": velocity, "x": x, "y": y, "z": z
                                                  });
            ball.clicked.connect(removeBall)
            // TODO: enforce uniqueness of keys.  Don't insert ball if its key is already in use.
            ObjectStore.insert(ball.uniqueId, ball)
        }

        onRemove: {
            console.debug("Received signal: REMOVE ", uuid)
            var ball = ObjectStore.remove(uuid)
            if (null != ball) ball.destroy()
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            var ball = itemComponent.createObject(page,
                                                  {"duration": 4000, "uniqueId":appLogic.uuid(),
                                                   "velocity": 100, "x": mouseX, "y": mouseY, "z": 3
                                                  });
            ball.clicked.connect(removeBall)
            ObjectStore.insert(ball.uniqueId, ball);
            console.debug("Emitting signal ADD ", ball.uniqueId, ball.color, ball.duration, ball.velocity, ball.x, ball.y, ball.z)
            appLogic.on_add(ball.uniqueId, ball.color, ball.duration, ball.velocity, ball.x, ball.y, ball.z)
        }
    }
}
