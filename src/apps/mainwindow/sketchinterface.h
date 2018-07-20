#ifndef SKETCHINTERFACE_H
#define SKETCHINTERFACE_H

#include <QDockWidget>

#include "sketching/sketchwindow.h"
#include "sketching/sketchingcontroller.h"


class MainWindow1;

class SketchInterface: public QObject
{
    Q_OBJECT

public:

  SketchInterface( MainWindow1* const& window_ );

  void createInterface();
  void init();


protected:

  void createSketchingWindow();
  void createSketchingActions();


protected:

    MainWindow1* window = nullptr;

    SketchingController* scontroller = nullptr;

    SketchWindow* sketch_window = nullptr;
    SketchWindow* sketch_topview_window = nullptr;

    QDockWidget* dw_sketchwindow = nullptr;
    QDockWidget* dw_topview_window = nullptr;

    QAction* ac_csection = nullptr;
    QAction* ac_topview = nullptr;


};

#endif // SKETCHINTERFACE_H
