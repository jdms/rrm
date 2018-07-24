#ifndef VIEW3DINTERFACE_H
#define VIEW3DINTERFACE_H


#include <QDockWidget>

#include "3dview/canvas3d.h"
#include "3dview/view3dcontroller.h"
#include "./core/widgets/realfeaturedslider.h"


class MainWindow;

class View3dInterface: public QObject
{
    Q_OBJECT

  public:

    View3dInterface( MainWindow* const& window_ );
    void createInterface();
    void init();

  protected:

    void createView3dWindow();
    void createView3dActions();


  protected:

    MainWindow* window = nullptr;

    View3dController* controller3d = nullptr;
    Canvas3d* canvas3d = nullptr;

    RealFeaturedSlider* sl_depth_csection = nullptr;

};

#endif // VIEW3DINTERFACE_H
