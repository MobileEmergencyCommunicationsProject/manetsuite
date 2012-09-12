#!/bin/sh
# usage: norm-mgen-sender interface-name
#
# e.g. norm-mgen-sender wlan0

mgen event "on 1 sink src 5001 dst 0.0.0.0/1 periodic [32 128]" output /dev/null sink STDOUT | norm addr 224.225.226.227/6789 interface $1 minput STDIN loopback on
