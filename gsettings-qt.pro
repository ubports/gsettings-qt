TEMPLATE = subdirs
SUBDIRS += \
    libgsettings-qt \
    gsettings-qt \
    tests \
    cpptest

libgsettings-qt.file = src/gsettings-qt.pro
gsettings-qt.file = GSettings/gsettings-qt.pro
gsettings-qt.depends = libgsettings-qt

tests.file = tests/tests.pro
cpptest.file = tests/cpptest.pro
cpptest.depends = libgsettings-qt
