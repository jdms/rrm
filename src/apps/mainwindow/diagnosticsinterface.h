#ifndef DIAGNOSTICSINTERFACE_H
#define DIAGNOSTICSINTERFACE_H

#include <QDockWidget>

class MainWindow;
class FlowWindow;


/**
 *   Class responsible for initializing the diagnostics application
 */


class DiagnosticsInterface: public QObject
{
    Q_OBJECT

    public:

        /**
        * Constructor.
        * @param window_ a const reference to the MainWindow
        * @see MainWindow
        */
        DiagnosticsInterface( MainWindow* const& window_ );


        /**
        * Method to creates the interface of the diagnostics application
        * @return void.
        */
        void createInterface();


    public slots:


        /**
        * Method to set the inital setup for the Flow Diagnostics app.
        * @param status_ boolean to indicate if the Flow Diagnostics is active or not.
        * @return void.
        */
        void init( bool status_ );

    protected:


        /**
        * Method to create the Flow Diagnostics interface
        * @return void.
        */
        void createDiagnosticsWindow();


        /**
        * Method to initialize and create the connects related to the actions of the Flow Diagnostics application
        * @return void.
        */
        void createDiagnosticsActions();


    protected:


        MainWindow* window = nullptr;                                       /**< Main Window */

        FlowWindow* flow_window = nullptr;                                 /**< Flow Diagnostics window */

        QDockWidget* dw_flow_window = nullptr;                             /**< DockWidget to hold the Flow Diagnostics window */


};

#endif // DIAGNOSTICSINTERFACE_H
