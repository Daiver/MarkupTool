!ALGLIB_PRI {

CONFIG += ALGLIB_PRI

INCLUDEPATH += $$PWD

HEADERS += \
	$$PWD/alglibinternal.h \
    $$PWD/alglibmisc.h \
    $$PWD/ap.h \
    $$PWD/dataanalysis.h \
    $$PWD/diffequations.h \
    $$PWD/fasttransforms.h \
    $$PWD/integration.h \
    $$PWD/interpolation.h \
    $$PWD/linalg.h \
    $$PWD/optimization.h \
    $$PWD/solvers.h \
    $$PWD/specialfunctions.h \
    $$PWD/statistics.h \
    $$PWD/stdafx.h

SOURCES += \
    $$PWD/alglibinternal.cpp \
    $$PWD/alglibmisc.cpp \
    $$PWD/ap.cpp \
    $$PWD/dataanalysis.cpp \
    $$PWD/diffequations.cpp \
    $$PWD/fasttransforms.cpp \
    $$PWD/integration.cpp \
    $$PWD/interpolation.cpp \
    $$PWD/linalg.cpp \
    $$PWD/optimization.cpp \
    $$PWD/solvers.cpp \
    $$PWD/specialfunctions.cpp \
    $$PWD/statistics.cpp
}

