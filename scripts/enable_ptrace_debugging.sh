#!/bin/sh
#
# Some Linux kernels disable ptrace debugging by default.
# This script turns it on temporarily. After you run this
# script ptrace debugging is enabled until you disable
# it by writing a 1 into /proc/sys/kernel/yama/ptrace_scope
# or until you reboot.
#
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

