#ifndef VIEW3DINTERFACE_H
#define VIEW3DINTERFACE_H


#include <QDockWidget>

#include "3dview/canvas3d.h"
#include "3dview/view3dcontroller.h"
#include "./core/widgets/realfeaturedslider.h"


class MainWindow;



/**
 *   Class responsible for initializing the 3d application
 */


class View3dInterface: public QObject
{
    Q_OBJECT

    public:


        /**
        * Constructor.
        */
        View3dInterface( MainWindow* const& window_ );


        /**
        * Method to creates the interface of the 3d application
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createInterface();


    public slots:


        /**
        * Method to initialize the controller.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void init();

    protected:


        /**
        * Method to create the 3d window interface
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createView3dWindow();


        /**
        * Method to initialize and create the connects related to the actions of the 3d application
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createView3dActions();


    protected:

        MainWindow* window = nullptr;                                   /**< Main Window */

        View3dController* controller3d = nullptr;                       /**< 3d Controller */

        Canvas3d* canvas3d = nullptr;                                   /**< 3d Canvas */

        RealFeaturedSlider* sl_depth_csection = nullptr;                /**< Cross-section slider */

};

#endif // VIEW3DINTERFACE_H
