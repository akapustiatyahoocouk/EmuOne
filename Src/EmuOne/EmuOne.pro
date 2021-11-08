TEMPLATE = subdirs

SUBDIRS += \
    emuone \
    emuone-cereon \
    emuone-core \
    emuone-ibm360 \
    emuone-scp360 \
    emuone-util

emuone.depends = emuone-core emuone-util
emuone-core.depends = emuone-util
emuone-ibm360.depends = emuone-core emuone-util
emuone-scp360.depends = emuone-ibm360 emuone-core emuone-util
