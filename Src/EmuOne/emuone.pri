QT += core gui widgets xml
CONFIG += c++17

INCLUDEPATH += ..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -Werror -ansi -Wall -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Weffc++ -Wextra -Wformat=2 -Winit-self -Wmissing-noreturn -Wno-long-long -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wsign-promo -Wswitch-default -Wswitch-enum -Wundef -Wunused -Wconversion -Wformat-security -Wformat-y2k -Wmissing-declarations -Wmissing-field-initializers -Wmissing-include-dirs -Wpacked -Wshadow -Wvariadic-macros -Wwrite-strings -fno-strict-aliasing -Wno-c++0x-compat
QMAKE_CXXFLAGS += -zdefs
QMAKE_LFLAGS += -zdefs

CONFIG(debug,debug|release) DESTDIR = ../../../Bin/Debug
CONFIG(release,debug|release) DESTDIR = ../../../Bin/Release

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

