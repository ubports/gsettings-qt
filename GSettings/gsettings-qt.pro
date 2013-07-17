TEMPLATE = lib
INCLUDEPATH += .
QT += qml
QT -= gui
CONFIG += qt plugin no_keywords link_pkgconfig
PKGCONFIG += gio-2.0

TARGET = GSettingsQmlPlugin

HEADERS = plugin.h gsettings-qml.h qconftypes.h util.h
SOURCES = plugin.cpp gsettings-qml.cpp qconftypes.cpp util.cpp

uri = GSettings

# deployment rules for the plugin
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
target.path = $$installPath
INSTALLS += target

extra.path = $$installPath
extra.files += qmldir
INSTALLS += extra
