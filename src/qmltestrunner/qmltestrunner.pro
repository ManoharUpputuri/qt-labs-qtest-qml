TEMPLATE = app
TARGET = qmltestrunner
CONFIG += warn_on
SOURCES += main.cpp
include(../quicktestlib/quicktestlib_dep.pri)

contains(QT_CONFIG, opengl)|contains(QT_CONFIG, opengles1)|contains(QT_CONFIG, opengles2) {
    QT += opengl
}

target.path = $$[QT_INSTALL_BINS]
INSTALLS += target
