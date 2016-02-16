TEMPLATE = app
CONFIG += console c++11

QT+=qml quick widgets 3dcore 3dinput 3drender 3dquick

SOURCES += main.cpp \
    resistor.cpp \
    source.cpp \
    calc.cpp \
    component.cpp \
    wire.cpp

HEADERS += \
    calc.h \
    resistor.h \
    source.h \
    component.h\
    wire.h



DISTFILES += \
    solution.png \
    Qml/AppView.qml \
    Qml/InputController.qml \
    Qml/World3D.qml \
    Qml/Source.qml \
    Qml/Generator.qml \
    Qml/Resistor.qml \
    Qml/GroundPlane.qml \
    assets/inputfile_new.sj

RESOURCES += \
    qml.qrc \
    resource.qrc

INCLUDEPATH += $$PWD/Eigen/
