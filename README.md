MANET Suite
====

The MANET Suite is about group communication over WIFI using mobile phones. 
It is a collection of free, open source applications.

The MANET Suite builds on the mobile ad hoc networking projects at the
U. S. Naval Research Laboratory.  


BUILDING MANET SUITE

To build the suite, you need QtCreator.  If you want to build for
Nokia phones, you also need Nokia's MADDE cross development tool. If
you use the version of QtCreator that comes with Nokia's Qt SDK, then
you already have MADDE.

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

