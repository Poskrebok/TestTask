
QT += sql
QT += charts

INCLUDEPATH += \
    $$PWD/widgets \
    $$PWD/db \
    $$PWD/plots \

SOURCES += \
        $$PWD/db/maindb.cpp \
        $$PWD/widgets/dialogs.cpp \
        $$PWD/main.cpp \
        $$PWD/plots/plotcontroller.cpp \
        $$PWD/widgets/familyview.cpp \
        $$PWD/widgets/mainwindow.cpp

HEADERS += \
    $$PWD/db/maindb.h \
    $$PWD/widgets/common.h \
    $$PWD/widgets/dialogs.h \
    $$PWD/plots/plotcontroller.h \
    $$PWD/widgets/familyview.h \
    $$PWD/widgets/mainwindow.h
