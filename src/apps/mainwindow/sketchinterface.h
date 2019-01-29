#ifndef SKETCHINTERFACE_H
#define SKETCHINTERFACE_H

#include <QDockWidget>

#include "sketching/sketchwindow.h"
#include "sketching/sketchingcontroller.h"


class MainWindow;


/**
 * \brief Class responsible for initializing the 3d application
 */


class SketchInterface: public QObject
{
    Q_OBJECT

    public:

        //TODO: add comments to the param

        /**
        * \brief Constructor.
        * @param window_
        */
        SketchInterface( MainWindow* const& window_ );


        /**
        * \brief Method to creates the interface of the Sketch application
        * @return void.
        */
        void createInterface();


    public slots:


        /**
        * \brief Method to initialize the controller and set initial setup.
        * @return void.
        */
        void init();


        /**
        * \brief Method to indicate if the user can see both canvas ( main and top-view canvas ) or not.
        * @param status_ if true both windows will be hidded. If false, both windows will be visible to the user.
        * @return void.
        */
        void showOnlyMainCanvas( bool status_ );


    protected:


        /**
        * \brief Method to create the Sketch window interface
        * @return void.
        */
        void createSketchingWindow();


        /**
        * \brief Method to initialize and create the connects related to the actions of the sketch application
        * @return void.
        */
        void createSketchingActions();


        /**
        * \brief Method to initialize and create the connects related to the actions of the sketch window application
        * @see sketch_window
        * @return Voidvoid
        */
        void createSketchWindowActions();


        /**
        * \brief Method to initialize and create the connects related to the actions of the top view sketch window application
        * @see sketch_topview_window
        * @return void.
        */
        void createSketchToViewWindowActions();


        /**
        * \brief Method to initialize and create the connects related to the actions of the mainwindow application
        * @return void.
        */
        void createMainWindowActions();


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
