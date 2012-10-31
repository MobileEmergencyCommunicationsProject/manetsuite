MANET Suite
====

Updated October 31, 2012


The MANET Suite is about group communication over WIFI using mobile phones. 
It is a collection of free, open source applications.

The MANET Suite builds on the mobile ad hoc networking projects at the
U. S. Naval Research Laboratory.  

OBTAINING MANET SUITE

MANET Suite is stored on GitHub in the repository git@github.com:MobileEmergencyCommunicationsProject/manetsuite.git.

The master branch will always be buildable and will always contain released code.
Releases are tagged in the form Release-<major>.<minor>.  For example, Release 1.0
is tagged Release-1.0.

The dev branch is for developers.  It might contain buildable, working code.


BUILDING MANET SUITE

To build the suite, you need QtCreator.  If you want to build for
Nokia phones, you also need Nokia's MADDE cross development tool. If
you use the version of QtCreator that comes with Nokia's Qt SDK, then
you already have MADDE.

CAUTION! When you load manetsuite.pro into QtCreator, QtCreator might
offer to update the deployment information for norm, protolib,
qnormtransport, and qpipe. Do not let it update those projects.  For
the present, this project builds only static libraries.  If you change
the deployment information for these libraries, QtCreator will deploy
them to your mobile devices.  This should be benign.  However,
including these libraries in the deployment package makes the package
too big to stage on Nokia N9 phones and so deployment will fail.
Further deployment of almost any other package will also fail until
you remove /tmp/manetsuite_0.0.1_armel.deb. This is a bug in
QtCreator.

To build the suite, you need recent snapshots of mgen, norm, nrlolsr,
nrlsmf, and protolib.  You can obtain them from
http://cs.itd.nrl.navy.mil/products.

Here are links to the nightly snapshots of those products:

mgen: http://downloads.pf.itd.nrl.navy.mil/mgen/nightly_snapshots/mgen-svnsnap.tgz

norm: http://downloads.pf.itd.nrl.navy.mil/norm/nightly_snapshots/norm-svnsnap.tgz

nrlolsr: http://downloads.pf.itd.nrl.navy.mil/olsr/nightly_snapshots/nrlolsr-svnsnap.tgz

nrlsmf: http://downloads.pf.itd.nrl.navy.mil/smf/nightly_snapshots/nrlsmf-svnsnap.tgz

protolib: http://downloads.pf.itd.nrl.navy.mil/protolib/nightly_snapshots/protolib-svnsnap.tgz


Unpack each snapshot into your manetsuite directory. This will
put the NRL code along side the QtCreator project files and other
additions provided by the MANET Suite.

Now use QtCreator to build the entire suite. The suite builds for and
runs on Linux, Maemo, and Meego/Harmattan. mgen, norm, nrlolsrd, and
nrlsmfd run in a command shell. The norm test tool, mping, runs on the
Linux, Maemo, and Meego/Harmattan desktops.

For Maemo and Meego/Harmattan, you may find that you need
libnetfilter_queue.a.  MANET Suite has been tested with the September
6, 2011 snapshot of libnetfilter_queue.  For now, you must install it
manually in your Maemo or Madde sysroot.

The suite does not build for Symbian Belle.  It lacks the means to
open the WIFI NIC in promiscuous mode and it needs a graphical front
end to nrlolsr and nrlsmf.


RUNNING MANET SUITE

To construct your MANET, you must

1. Choose addresses for each device in the MANET
2. Run nrlsmf
3. Run nrlolsrd


When you choose an address, be sure to put the device's NIC in ad hoc mode. 

To run nrlsmf, try this:

nrlsmf instance nrlsmf cf wlan0 hash MD5 debug 0 log /var/log/nrlsmf.log

You can set the debug level from 0 to 7.

The other options are explained in the nrlsmf user guide,
nrlsmf/nrlsmf.html and nrlsmf/nrlsmf.pdf.  Read it. It's nice.

Run nrlsmf before you start nrlolsrd.



To run nrlolsrd, try this:

nrlolsrd -i wlan0 -flooding ecds -d 5 -l /var/log/nrlolsrd.log -rpipe nrlolsr -smfClient nrlsmf

Set the debug level with the -d option.  As with nrlsmf, the values
range from 0 to 7.

You may find that nrlolsrd demands too much of your mobile device's
CPU.  You could try increasing the Hello interval to 2 and the TC
interval to 5.  The command line arguments for these changes are:

	 -hi 2 -tci 5


You will find further instructions for running nrlolsrd in
nrlolsr/readme.help.  Read it.  It's nice.


WHAT TO DO NEXT

Once you have the MANET running, you can watch the output of nrlolsrd
to see the routing tables it constructs.  You can read it in the
command shell.  If you are using a Nokia N9 phone or a Linux desktop
you can use olsrguiharmattan or olsrgui, respectively.  They are built
automatically for those platforms.

You can verify connectivity among the devices in your MANET with
mping, which uses NORM to guarantee delivery of small messages.

You can share files with the devices in your MANET with mcshare. It's
slow but it works.
