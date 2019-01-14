#ifndef SKETCHINTERFACE_H
#define SKETCHINTERFACE_H

#include <QDockWidget>

#include "sketching/sketchwindow.h"
#include "sketching/sketchingcontroller.h"


class MainWindow;


/**
 *   Class responsible for initializing the 3d application
 */


class SketchInterface: public QObject
{
    Q_OBJECT

    public:


        /**
        * Constructor.
        */
        SketchInterface( MainWindow* const& window_ );


        /**
        * Method to creates the interface of the Sketch application
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createInterface();


    public slots:


        /**
        * Method to initialize the controller and set initial setup.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void init();


        /**
        * Method to indicate if the user can see both canvas ( main and top-view canvas ) or not.
        * @param status_ if true both windows will be hidded. If false, both windows will be visible to the user.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void showOnlyMainCanvas( bool status_ );


    protected:


        /**
        * Method to create the Sketch window interface
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createSketchingWindow();


        /**
        * Method to initialize and create the connects related to the actions of the sketch application
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createSketchingActions();


    protected:

        MainWindow* window = nullptr;                                               /**< Main Window */

        SketchingController* scontroller = nullptr;                                 /**< Sketch Controller */

        SketchWindow* sketch_window = nullptr;                                      /**< Main cross-section canvas */

        SketchWindow* sketch_topview_window = nullptr;                              /**< Top View cross-section canvas */

        QDockWidget* dw_sketchwindow = nullptr;                                     /**< DockWidget to hold the main cross-section canvas */

        QDockWidget* dw_topview_window = nullptr;                                   /**< DockWidget to hold the top view cross-section canvas */

        QAction* ac_csection = nullptr;                                             /**< Action to turn the dw_sketchwindow visible/invisible */

        QAction* ac_topview = nullptr;                                              /**< Action to turn the dw_topview_window visible/invisible */


};

#endif // SKETCHINTERFACE_H
