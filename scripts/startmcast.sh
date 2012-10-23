#!/bin/sh
#
# Start the multicast experiment
#

if [ ! -z $1 ] ; then
  OLSRD="/opt/manetsuite/bin/nrlolsrd"
  SMFD="/opt/manetsuite/bin/nrlsmf"
else
  OLSRD="nrlolsrd"
  SMFD="nrlsmf"
fi
 
#
# It might be necessary to change some sysctl variables in order for
# nrlolsrd to receive OLSR messages on wlan0.  Use get_mcast_sysctl.sh
# to obtain the current values.  Save them to a file.  Use
# set_mcast_sysctl.sh to set the variables to their correct values.
# Use sysctl -p to restore the original values at the end of your
# experiment.  For example,
#
# get_mcast_sysctl.sh > originalvalues.conf
# set_mcast_sysctl.sh
# startmcast.sh
#
# ... your experiment ...
#
# stopmcast.sh
# sysctl -p originalvalues.conf
#

#
# DEVICE is the name of the interface
# on which to run nrlsmf and nrlolsrd
#
DEVICE="wlan0"

#
# Parameters for nrlolsrd
#
OLSRNAME="nrlolsr"
OLSR_DEBUG="-d 5"

#
# Parameters for nrlsmf
#
SMFNAME="nrlsmf"
SMF_DEBUG="debug 0"

$SMFD instance $SMFNAME cf $DEVICE hash MD5 \
$SMF_DEBUG \
log /var/log/nrlsmf.log &

sleep 10

$OLSRD -i $DEVICE -flooding ecds \
$OLSR_DEBUG \
-l /var/log/nrlolsrd.log \
-rpipe $OLSRNAME \
-smfClient $SMFNAME &
