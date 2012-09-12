scripts
====

The scripts directory contains shell scripts for running mgen alone,
mgen over NORM, and nrlolsrd and nrlsmf. These are test scripts.

get_mcast_sysctl.sh: Prints the values of the Linux sysctl names that
affect nrlolsrd and reading packets in promiscuous mode.  Use this to
save those values to a file. Restore those values with "sysctl -p".

mgen-recv.sh: Receive mgen data on the multicast address 224.225.226.227/5000.

mgen-send.sh: Send mgen data on the multicast address 224.225.226.227/5000.

norm-mgen-recv.sh: Receive mgen data over NORM on the multicast
address 224.225.226.227/5000.

norm-mgen-send.sh: Send mgen data over NORM on the multicast address
224.225.226.227/5000.

set_mcast_sysctl.sh: Changes the values of some Linux sysctl names so
that nrlolsrd can read packets in promiscous mode. 

startmcast.sh: Start nrlsmf and nrlolsrd.  On some Linux systems, you
may need to run get_mcast_sysctl.sh and set_mcast_sysctl.sh first. See
the comments inside startmcast.sh for instructions.

stopmcast.sh: Stop nrlsmf and nrlolsrd.