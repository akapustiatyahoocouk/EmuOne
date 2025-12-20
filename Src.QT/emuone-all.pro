TEMPLATE = subdirs

SUBDIRS +=   \
    emuone \
    emuone-core \
    emuone-util

emuone.depends = emuone-core emuone-util
emuone-core.depends = emuone-util
