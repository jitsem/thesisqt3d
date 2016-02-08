TEMPLATE = app
CONFIG += console c++11

QT+=qml quick widgets

SOURCES += main.cpp \
    resistor.cpp \
    source.cpp \
    calc.cpp \
    component.cpp

HEADERS += \
    calc.h \
    resistor.h \
    source.h \
    component.h\



DISTFILES += \
    drawing \
    AppView.qml

RESOURCES += \
    qml.qrc \
    resource.qrc

INCLUDEPATH += $$PWD/Eigen/
