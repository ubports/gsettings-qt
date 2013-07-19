TEMPLATE = lib
INCLUDEPATH += .
QT -= gui
CONFIG += qt no_keywords link_pkgconfig
PKGCONFIG += gio-2.0

TARGET = gsettings-qt

HEADERS = qgsettings.h util.h qconftypes.h
SOURCES = qgsettings.cpp util.cpp qconftypes.cpp

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

headers.path = $$[QT_INSTALL_HEADERS]
headers.files = qgsettings.h QGSettings
INSTALLS += headers
