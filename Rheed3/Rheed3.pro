QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_VERSION, 5.1): QT += quick

debug: TARGET = Rheed3.1_D
release: TARGET = Rheed3.1
TEMPLATE = app
-
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

debug : DEFINES += "DEBUG"


CONFIG += c++11

SOURCES += \
    Globals/globals.cpp \
    #QCustomPlot/qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Globals/globals.h \
    #QCustomPlot/qcustomplot.h \
    mainwindow.h

#DISTFILES += \
 #   QCustomPlot/GPL.txt \
  #  QCustomPlot/changelog.txt
