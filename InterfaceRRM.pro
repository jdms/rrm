QT += widgets
QT += opengl

TEMPLATE = app

!host_build:QMAKE_MAC_SDK = macosx10.9

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


CONFIG += c++11

unix:mac {
    QXXFLAGS += -std=c++11 -mmacosx-version-min=10.9 -stdlib=libc++;
}


SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Canvas2D.cpp \
    Canvas3D.cpp \
    SketchingScene.cpp \
    InputSketch.cpp \
    HorizonController.cpp \
    BoundaryController.cpp \
    canvasComputation.cpp \
    FlowVisualizationController.cpp \
    VTKData.cpp \
    ColorMap.cpp

HEADERS += \
    MainWindow.h \
    Canvas2D.h \
    Canvas3D.h \
    SketchingScene.h \
    InputSketch.h \
    HorizonController.h \
    BoundaryController.h \
    canvasComputation.h \
    FlowVisualizationController.h \
    VTKData.h \
    ColorMap.h

DISTFILES += \
    vertex_shader.vert \
    fragment_shader.frag

OTHER_FILES += \
    fragment_shader.frag \
    vertex_shader.vert
