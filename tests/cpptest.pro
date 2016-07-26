TEMPLATE = app
QT += testlib
QT -= gui
CONFIG += testcase link_pkgconfig
TARGET = cpptest
IMPORTPATH = $$PWD/..
SOURCES = cpptest.cpp
INCLUDEPATH += $$(PWD)/../src
LIBS += -L$$(PWD)/../src -lgsettings-qt

schema.target = gschemas.compiled
schema.commands = glib-compile-schemas $$PWD
schema.depends = com.canonical.gsettings.test.gschema.xml
QMAKE_EXTRA_TARGETS += schema
PRE_TARGETDEPS = gschemas.compiled

