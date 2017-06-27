#NoRegrets

QT       += core gui
QT       += webenginewidgets
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoRegrets
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        browser.cpp \
        instrument.cpp \
        guessingmechanism.cpp \
        instrumentswindow.cpp \
    instrumentlibrary.cpp \
    analysiswindow.cpp \
    mychart.cpp \
    config.cpp

HEADERS  += mainwindow.h \
        browser.h \
        instrument.h \
        guessingmechanism.h \
        instrumentswindow.h \
    instrumentlibrary.h \
    analysiswindow.h \
    mychart.h \
    config.h

FORMS    += mainwindow.ui \
        instrumentswindow.ui \
    analysiswindow.ui

RESOURCES += \
        resources.qrc
