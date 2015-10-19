QT += widgets
QT += opengl

TEMPLATE = app


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
    FlowVisualizationController.cpp

HEADERS += \
    MainWindow.h \
    Canvas2D.h \
    Canvas3D.h \
    SketchingScene.h \
    InputSketch.h \
    HorizonController.h \
    BoundaryController.h \
    canvasComputation.h \
    FlowVisualizationController.h
