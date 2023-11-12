QT       += core gui printsupport
QMAKE_CXXFLAGS += -O2
QMAKE_CXXFLAGS += -k8
QMAKE_CXXFLAGS += -no-pie
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MathPart/emperical_distribution.cpp \
    MathPart/main_distribution.cpp \
    MathPart/mixture_distribution.cpp \
    MathPart/spec_func.cpp \
    qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MathPart/emperical_distribution.h \
    MathPart/main_distribution.h \
    MathPart/mixture_distribution.h \
    MathPart/parameters.h \
    MathPart/spec_func.hpp \
    qcustomplot.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
