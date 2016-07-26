TEMPLATE = app
QT += qml testlib
QT -= gui
CONFIG += qmltestcase
TARGET = test
IMPORTPATH = $$PWD/..
SOURCES = test.cpp

schema.target = gschemas.compiled
schema.commands = glib-compile-schemas $$PWD
schema.depends = com.canonical.gsettings.test.gschema.xml
QMAKE_EXTRA_TARGETS += schema
PRE_TARGETDEPS = gschemas.compiled

OTHER_FILES += tst_GSettings.qml
