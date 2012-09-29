TEMPLATE = subdirs

SUBDIRS = \
    mgen \
    mping \
    norm \
    nrlolsr \
    nrlsmf \
    normApp \
    protolib \
    qnormtransport \
    qnormtransporttest \
    qnormfiletransporttest \
    normFileRecv \
    normFileSend \
    mcshare \
    qnormtransport/normfileinfoiteratortest

unix:!symbian {
    maemo5 {
# Don't build olsrgui for maemo5
    } else:contains(MEEGO_EDITION,harmattan) {
# Don't build olsrgui for meego harmattan
    } else {
	SUBDIRS += olsrgui
	olsrgui.depends += protolib
    }
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

mgen.depends = protolib
mping.depends = qnormtransport norm protolib
normfileinfoiteratortest.depends = qnormtransport norm protolib
nrlolsr.depends = protolib
nrlsmf.depends = protolib
qnormfiletransporttest = qnormtransport norm protolib
qnormtransport.depends = norm protolib
qnormtransporttest.depends = qnormtransport norm protolib
