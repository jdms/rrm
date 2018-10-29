#ifndef DIAGNOSTICSINTERFACE_H
#define DIAGNOSTICSINTERFACE_H

#include <QDockWidget>

class MainWindow;
class FlowWindow;


class DiagnosticsInterface: public QObject
{
    Q_OBJECT

public:

    DiagnosticsInterface( MainWindow* const& window_ );

    void createInterface();


public slots:

    void init( bool status_ );

protected:

    void createDiagnosticsWindow();
    void createDiagnosticsActions();


protected:

    MainWindow* window = nullptr;

    FlowWindow* flow_window = nullptr;
    QDockWidget* dw_flow_window = nullptr;


};

#endif // DIAGNOSTICSINTERFACE_H
