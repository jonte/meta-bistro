#
#   Copyright (C) 2016 Pelagicore AB
#
DESCRIPTION = "Helpers for installing different multimedia types"

# Known media types. Add extensions to create packages for the new extensions
FILETYPES_AUDIO = "mp3 aac m4a ac3 ogg wma au mod wav flac"
FILETYPES_VIDEO = "avi mov mpg wmv sid swf asf dat dv scf"
FILETYPES_IMAGE = "png jpg gif"

# Add the corresponding variable above if adding new media types to this list.
# Removing from the list prevents packages of the given type to be built.
FILETYPES_PACKS = "audio video image"

S = "${WORKDIR}"

python () {
    """ Create a FILETYPES variable which contains all filetypes """
    ftypes = ""
    for pack in d.getVar("FILETYPES_PACKS", True).split(" "):
        ftypes += d.expand("${FILETYPES_"+pack.upper()+"}")
    d.setVar("FILETYPES", ftypes)
}

do_install() {
    for ext in ${FILETYPES}
    do
        mkdir -p ${D}${datadir}/testmedia/$ext
        find ${WORKDIR} -name "*.${ext}" -print0 | xargs -0 -i install -m 644 "{}" ${D}${datadir}/testmedia/$ext
    done
}

python populate_packages_prepend () {
    """ Create an empty package with rdepends to over of the filetype groups.
        This allows creating an 'audio' or 'video' package which RDEPENDs on
        all audio/video formats

        ftype_var: FILETYPES_AUDIO, FILETYPES_VIDEO, etc.
        pkg_name: Suffix to package, 'audio', 'video', etc
    """
    def create_virtual(ftype_var, pkg_name):
        pn = d.expand("${PN}-" + pkg_name)

        rdeps = ""
        for ftype in d.getVar(ftype_var, True).split(" "):
            rdep = d.expand("${PN}-" + ftype)
            rdeps = rdeps + " " + rdep
        d.setVar("RDEPENDS_" + pn, rdeps)
        d.setVar("ALLOW_EMPTY_" + pn, "1")

        return " " + pn + " "

    # Create packages for all different filetypes
    newpackages = ""
    ftypes = d.getVar('FILETYPES', True)
    for ftype in ftypes.split(" "):
        newpackages = newpackages + d.expand(' ${PN}-' + ftype)
        d.setVar(d.expand('FILES_${PN}-' + ftype), d.expand('${datadir}/testmedia/' + ftype))

    # Create virtual packages for grouping different kinds of filetypes
    for pack in d.getVar("FILETYPES_PACKS", True).split(" "):
        newpackages += create_virtual("FILETYPES_" + pack.upper(), pack)

    # Virtual package containing all other virtual packages
    newpackages = newpackages + create_virtual("FILETYPES_PACKS", "all")

    d.setVar('PACKAGES', newpackages)
}
