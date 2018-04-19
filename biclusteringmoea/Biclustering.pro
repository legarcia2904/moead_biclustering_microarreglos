TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    global.cpp \
    individual.cpp \
    subproblem.cpp \
    moead.cpp \
    crossover.cpp \
    mutation.cpp

HEADERS += \
    global.h \
    individual.h \
    moead.h \
    subproblem.h \
    common.h \
    crossover.h \
    mutation.h \
    scalarfunc.h





