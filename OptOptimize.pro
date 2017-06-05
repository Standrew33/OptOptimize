#-------------------------------------------------
#
# Project created by QtCreator 2016-10-30T21:45:57
#
#-------------------------------------------------

QT       += core gui sql

QT  += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = OptOptimize
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    genes.cpp \
    population.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    genes.h \
    population.h \
    qcustomplot.h

FORMS    += mainwindow.ui
