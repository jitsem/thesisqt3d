TEMPLATE = app
CONFIG += console c++11

QT+=qml quick widgets 3dcore 3dinput 3drender 3dquick xml svg

QTPLUGIN += qsvg@

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
    Qml/InputController.qml \
    Qml/World3D.qml \
    Qml/Source.qml \
    Qml/Generator.qml \
    Qml/Resistor.qml \
    Qml/GroundPlane.qml \
    Qml/Wire.qml \
    Qml/Electron.qml \
    Qml/CircuitView.qml \
    Qml/Main.qml \
    Qml/EditMenu.qml \

RESOURCES += \
    qml.qrc \
    resource.qrc

INCLUDEPATH += $$PWD/Eigen/
