#!/bin/bash
#
# Print the mcast related sysctl variables.
# The output is suitable as input to sysctl -p
# E.g.
#
# get_mcast_sysctl.sh > foo.conf
# sysctl -p foo.conf
#

declare -a SYSCTL_NAMES

SYSCTL_NAMES=(net.ipv4.conf.all.forwarding \
net.ipv6.conf.all.forwarding \
net.ipv4.conf.all.send_redirects \
net.ipv4.conf.all.rp_filter \
net.ipv4.conf.default.rp_filter \
net.ipv4.conf.wlan0.forwarding \
net.ipv4.conf.wlan0.send_redirects \
net.ipv4.conf.wlan0.rp_filter )


#
# REMEMBER: "${array[@]}" expands each element of
# array as a single word.
#
for N in "${SYSCTL_NAMES[@]}" ; do 
    echo -n "$N="
    sysctl -n $N
done
