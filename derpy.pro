TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    ircclient.cpp

RESOURCES += qml.qrc #\
#    qml_plugins.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    ircclient.h

win32:RC_ICONS = DERPY_HOOVES_ICON.ico

#QMAKE_LFLAGS += -static -static-libgcc

DISTFILES += \
    README.md
