QNormTransportTest
====

qnormtransporttest is a simple, command line test program for
QNormTransportStream.

It works in two modes: sender and receiver.  Command line usage is either

qnormtransporttest -s [interface-name]

or 

qnormtransporttest -r [interface-name]

Where the "-s" indicates sending and "-r" indicates receiving.  By
default, qnormtransporttest sends or receives on the wlan0 interface.

The sender sends a series of short messages at one-second intervals on
the multicast address 224.1.2.3/6003.

The receiver prints the messages as they arrive.

