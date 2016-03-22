#-------------------------------------------------
#
# Project created by QtCreator 2015-10-06T16:16:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExtractData
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    readmaus_mc.cpp \
    betterreadmaus.cpp \
    quadrupole.cpp \
    slicequad.cpp \
    stepibeamline.cpp \
    stepiiteration.cpp \
    stepitracking.cpp \
    tophat.cpp \
    units.cpp

HEADERS  += mainwindow.h \
    readmaus_mc.h \
    betterreadmaus.h \
    quadrupole.h \
    slicequad.h \
    stepibeamline.h \
    stepiiteration.h \
    stepitracking.h \
    tophat.h \
    units.h

FORMS    += mainwindow.ui

LIBS += -L${MAUS_ROOT_DIR}/third_party/build/root/lib -lCint -lCore -lMathCore
LIBS += -lMathMore -lHist -lTree -lMatrix -lRIO -lThread
LIBS += -lGui -lRIO -lNet -lGraf -lGraf3d -lGpad -lRint -lPostscript -lPhysics -lThread -pthread -lm -ldl -rdynamic
LIBS += -L${MAUS_ROOT_DIR}/src/common_cpp -lMausCpp
LIBS += -L${MAUS_ROOT_DIR}/third_party/install/lib
LIBS += -ljson -lPhysics
LIBS += -lCLHEP
LIBS += -lG4geometry -lG4graphics_reps -lG4materials -lG4particles
LIBS += -lG4processes -lG4run -lG4event -lG4global -lG4intercoms
LIBS += -lG4modeling -lG4tracking -lG4visHepRep -lG4VRML -lG4digits_hits
LIBS += -lG4FR -lG4physicslists -lG4vis_management -lG4clhep -lG4track -lG4zlib


INCLUDEPATH += ${MAUS_ROOT_DIR}/third_party/build/root/include/


INCLUDEPATH += ${MAUS_ROOT_DIR}/src/common_cpp
INCLUDEPATH += ${MAUS_ROOT_DIR}
INCLUDEPATH += ${MAUS_ROOT_DIR}/src/legacy
INCLUDEPATH += ${MAUS_ROOT_DIR}/third_party/install/include



OTHER_FILES += \
    readData.py
