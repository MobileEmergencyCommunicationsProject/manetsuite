QNormFileTransportTest
====

qnormfiletransporttest is a command line test program for
QNormFileTransport.

It works in two modes: sender and receiver.  Command line usage is either

qnormtransporttest -s interface-name list-of-files

or

qnormtransporttest -r [interface-name]

Where the "-s" indicates sending and "-r" indicates receiving.

In reciver mode, the default interface is wlan0.  qnormtransporttest
creates the directory NormFileTransportTest in the platform standard
directory for documents.  On Linux desktop systems, it is usually
$HOME/Documents.  On Meego/Harmattan phones, it is usually
$HOME/MyDocs/Documents.  On Maemo phones, it is usually
$HOME/MyDocs/.documents.

In sender mode, the interface name is a required command line argument.
The list-of-files can include directories.  qtnormtransporttest sends
all readable files named on the command line or contained in directory
trees named on the command line.

qnormfiletransporttest reads or writes on the multicast address 224.1.2.3/6003.

The program prints some messages of its progress as a sender or a receiver.
