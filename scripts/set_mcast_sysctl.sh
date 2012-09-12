#!/bin/bash
#
# Change the mcast related sysctl variables so that OLSR messages,
# among others, can be read on wlan0.
#
# Consider saving the current values of those variables first by
# running get_mcast_sysctl.sh, saving its output to a file.  Then when
# you complete your mcast work, you can restore those settings using
# sysctl -p.  E.g.
#
# ./get_mcast_sysctl.sh > oldsettings.conf
# ./set_mcast_sysctl.sh
#
# ... do your work ..
#
# sysctl -p oldsettings.conf
#

declare -A SYSCTL_NAMES

SYSCTL_NAMES=([net.ipv4.conf.all.forwarding]=1 \
[net.ipv6.conf.all.forwarding]=1 \
[net.ipv4.conf.all.send_redirects]=0 \
[net.ipv4.conf.all.rp_filter]=0 \
[net.ipv4.conf.default.rp_filter]=0 \
[net.ipv4.conf.wlan0.forwarding]=1 \
[net.ipv4.conf.wlan0.send_redirects]=0 \
[net.ipv4.conf.wlan0.rp_filter]=0 )


#
# REMEMBER: "${array[@]}" expands each element of
# array as a single word.
#
for N in "${!SYSCTL_NAMES[@]}" ; do 
    sysctl -w "$N=${SYSCTL_NAMES[$N]}"
done
