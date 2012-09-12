mping
====

mping is a finger-friendly tool for testing NORM, the Nack Oriented
Reliable Multicast protocol, on Linux desktops and on mobile phones.

USING MPING:

Run it on a few devices to test the NORM protocol.  mping is
configured to use the interface called "WLAN0", which is typically a
WIFI interface on Linux systems.

mping's simple user interface has three buttons ("RECEIVE", "SEND",
and "STOP") and a scrollable display area.  Choose one of your devices
as the sender and push its "SEND" button. On the other devices, push
the "RECEIVE" button.

The sending device sends a series of short messages on the multicast
address 224.1.2.3/6003.  It displays each message in the display area.
The messages alternate in color between red and green.

The receiving devices print the messages they receive in the display
area.  The messages that arrive at the same time are printed in the
same color.  The colors alternate between red and green.
