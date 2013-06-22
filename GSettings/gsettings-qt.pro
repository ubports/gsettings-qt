TEMPLATE = lib
INCLUDEPATH += .
QT += qml
QT -= gui
CONFIG += debug link_pkgconfig
PKGCONFIG += gio-2.0
DEFINES += QT_NO_KEYWORDS

TARGET = GSettingsQmlPlugin

HEADERS = plugin.h gsettings-qml.h qconftypes.h
SOURCES = plugin.cpp gsettings-qml.cpp qconftypes.cpp

uri = GSettings

# deployment rules for the plugin
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
target.path = $$installPath
INSTALLS += target

extra.path = $$installPath
extra.files += qmldir
INSTALLS += extra
