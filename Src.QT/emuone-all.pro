TEMPLATE = subdirs

SUBDIRS +=   \
    emuone-core \
    emuone-util

emuone-core.depends = emuone-util
