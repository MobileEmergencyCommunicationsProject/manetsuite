Balls! is a simple-minded, multiplayer game that tests QNormDataTransport.  The game is played in a window.  Players create balls of their own by touching an empty space.  Each new ball appears in the game window of each player.  Players destroy balls of other players by touching them.  The game doesn't end on its own.

Balls! communicates game state using the QNormDataTransport reliable multicast transport. It transmits each state change as a single message.  These are the messages:

ADD player-uuid ball-uuid velocity x y z
REMOVE player-uuid ball-uuid

Each player has a globally unique id, the player-uuid.
Each ball has a globally unique id, the ball-uuid; a velocity; and an initial position in three-space.

When a player creates a ball, the game sends an ADD message.  All other players' games add the ball to the display.

When a player destroys another player's ball, the game sends a REMOVE message.  All other players' games remove the ball from the display.

The player-uuid and ball-uuid are represented by null-terminated strings.
The velocity is a qint64, Qt's notion of a 64-bit signed integer.
The initial position is represented by three doubles, x, y, and z.

When the game receives an ADD message from a player it doesn't already know about, it sends an ADD message for each of its balls that it thinks are still in play. It is possible during this time that the game sends an ADD message for a ball that another player has destroyed.  The REMOVE message hasn't reached this instance of the game.
