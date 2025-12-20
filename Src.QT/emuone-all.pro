TEMPLATE = subdirs

SUBDIRS +=   \
    emuone \
    emuone-core \
    emuone-hades \
    emuone-util

emuone.depends = emuone-core emuone-util
emuone-core.depends = emuone-util

emuone-hades.depends = emuone-core emuone-util
