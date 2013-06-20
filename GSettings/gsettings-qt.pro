TEMPLATE = lib
INCLUDEPATH += .
QT += qml
CONFIG += debug link_pkgconfig
PKGCONFIG += gio-2.0
DEFINES += QT_NO_KEYWORDS

TARGET = GSettingsQmlPlugin

HEADERS = plugin.h gsettings-qml.h qconftypes.h
SOURCES = plugin.cpp gsettings-qml.cpp qconftypes.cpp
