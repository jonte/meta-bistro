#
#   Copyright (C) 2016 Pelagicore AB
#
#
DESCRIPTION = "GStreamer multimedia test files downloaded from Pelagicore mirror"

include test-multimedia.inc

HOMEPAGE = "https://139.162.170.90/public/"
LICENSE = "CC-BY-NC-1.0 | CC-BY-SA-1.0 | CC-BY-1.0"
SRC_URI = "https://testmedia.pelagicore.net/public/media.tar.gz;name=mediatgz"
SRC_URI[mediatgz.md5sum] = "995932416023a7777a133221a4ba3f66"
