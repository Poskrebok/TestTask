QT +=core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

include( $$PWD/src/src.pri )
    OPTIONS = standard custom
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




