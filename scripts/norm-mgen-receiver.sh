#!/bin/sh
# usage: norm-mgen-receiver interface-name
#
# e.g. norm-mgen-receiver wlan0
norm addr 224.225.226.227/6789 interface $1 moutput STDOUT loopback on | mgen source STDIN flush
