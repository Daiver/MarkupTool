!CURVES_PRI {
#-------------------------------------------------
#
# Project created by QtCreator 2018-04-19T16:27:43
#
#-------------------------------------------------


SOURCES +=

HEADERS +=

INCLUDEPATH += $$PWD/../

# DISTFILES += \
#     splines.bib

SOURCES += \
    $$PWD/bspline.cpp

HEADERS +=

DISTFILES += \
    $$PWD/splines.bib

HEADERS += \
    $$PWD/curves.h \
    $$PWD/impl/bspline_impl.h \
    $$PWD/impl/bspline_fit_impl.h \
    $$PWD/impl/bspline_interpolate_impl.h \
    $$PWD/impl/curve_proxy.h \
    $$PWD/impl/curve_samples.h \
    $$PWD/impl/curve_fit.h \
    $$PWD/impl/curve_unif_iter.h \
    $$PWD/impl/curvebspline_base.h \
    $$PWD/curvebezier.h \
    $$PWD/curvebspline.h

}

HEADERS += \
    $$PWD/math/bspline.h \
    $$PWD/math/catmullrom.h \
    $$PWD/curvecatmullrom.h \
    $$PWD/math/bsplinescomb_inverse.h \
    $$PWD/impl/bsplinescomb_inverse_impl.h \
    $$PWD/impl/curveinverse_base.h
