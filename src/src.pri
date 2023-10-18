
QT += sql
#QT += charts

INCLUDEPATH += \
    $$PWD/widgets \
    $$PWD/db \
    $$PWD/plots \

SOURCES += \
        $$PWD/db/maindb.cpp \
        $$PWD/db/common.cpp \
        $$PWD/widgets/dialogs.cpp \
        $$PWD/main.cpp \
        $$PWD/plots/plotcontroller.cpp \
        $$PWD/widgets/familymodel.cpp \
        $$PWD/widgets/mainwindow.cpp

HEADERS += \
    $$PWD/db/maindb.h \
    $$PWD/db/common.h \
    $$PWD/widgets/dialogs.h \
    $$PWD/plots/plotcontroller.h \
    $$PWD/widgets/familymodel.h \
    $$PWD/widgets/mainwindow.h
