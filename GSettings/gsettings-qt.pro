TEMPLATE = lib
QT += qml
QT -= gui
CONFIG += qt plugin no_keywords link_pkgconfig
PKGCONFIG += gio-2.0
INCLUDEPATH += ../src .
LIBS += -L../src -lgsettings-qt

TARGET = GSettingsQmlPlugin

HEADERS = plugin.h gsettings-qml.h
SOURCES = plugin.cpp gsettings-qml.cpp

uri = GSettings
API_VER = 1.0

# deployment rules for the plugin
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /).$$API_VER
target.path = $$installPath
INSTALLS += target

extra.path = $$installPath
extra.files += qmldir
INSTALLS += extra

qmltypes.path = $$installPath
qmltypes.files = plugins.qmltypes
qmltypes.extra = export LD_PRELOAD=../src/libgsettings-qt.so.1; $$[QT_INSTALL_BINS]/qmlplugindump -notrelocatable GSettings 1.0 .. > $(INSTALL_ROOT)/$$installPath/plugins.qmltypes
INSTALLS += qmltypes

