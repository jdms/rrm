#include "MainWindow.h"

void MainWindow::createComputationModule()
{


    dc_computation = new QDockWidget( this );
    dc_computation->setAllowedAreas( Qt::RightDockWidgetArea );
    dc_computation->setWindowTitle( "Flow Visualization" );

    QFrame *fr = new QFrame( this );
    fr->setFrameStyle( QFrame::Box | QFrame::Sunken );


    QGLFormat fmt;
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QGLFormat::CoreProfile);

    canvas_computation = new CanvasComputation( fmt, this );
    canvas_computation->setMinimumHeight( 350 );

    QHBoxLayout *hb_canvascomputation = new QHBoxLayout( this );
    hb_canvascomputation->addWidget( canvas_computation );

    fr->setLayout( hb_canvascomputation );

    mw_canvas_computation = new QMainWindow();
    mw_canvas_computation->setCentralWidget( fr );

    dc_computation->setWidget( mw_canvas_computation );
    dc_computation->setVisible( false );
    addDockWidget( Qt::RightDockWidgetArea, dc_computation );

    connect( ac_flowcomputation, SIGNAL( toggled(bool) ), dc_computation, SLOT( setVisible(bool) ) );

}


void MainWindow::createToolbarComputation()
{

    QLabel *lb_name_property = new QLabel( tr( "  Property  " ) );
    QLabel *wd_space = new QLabel("  ");

    tlb_workflow_flow = addToolBar( tr( "Workflow" ) );

    tlb_workflow_flow->addAction( ac_open_userinput );
//    tlb_workflow_flow->addAction( ac_open_surface );
    tlb_workflow_flow->addSeparator();

    tlb_workflow_flow->addAction( ac_compute_volumetric );
    tlb_workflow_flow->addAction( ac_compute_pressure );
    tlb_workflow_flow->addAction( ac_compute_velocity );
    tlb_workflow_flow->addAction( ac_compute_tof );

    tlb_workflow_flow->addSeparator();

    tlb_workflow_flow->addWidget( lb_name_property );
    tlb_workflow_flow->addWidget( cb_compute_property );



    tlb_workflow_flow->addWidget( wd_space );
    tlb_workflow_flow->addWidget( cb_coloroption_vector );

    mw_canvas_computation->addToolBar( tlb_workflow_flow );

}


void MainWindow::doComputation()
{
    addDockWidget( Qt::RightDockWidgetArea, dc_computation );
    dc_computation->show();

//    canvas_computation->showVolumetricGrid();
}



void MainWindow::clearComputation()
{
    canvas_computation->resetSetup();
    canvas_computation->updateGL();

}


void MainWindow::openUserInputFile()
{

    dg_inputuser->show();

    /*
    statusBar()->showMessage( "Open user input file" );

    QString filename = QFileDialog::getOpenFileName( this );
    if( filename.isEmpty() == true ) return;

    emit sendInputUserFile( filename.toStdString() );

    statusBar()->showMessage( "User input file loaded" );
    */


}

void MainWindow::createMeshVolumetric()
{
    emit computeVolume();
    canvas_computation->showVolumetricGrid();

    statusBar()->showMessage( "Volumetric meshing done" );
}

void MainWindow::selectProperty( int id )
{
    bool option = false;
    emit selectFlowProperty( id, option );
    cb_coloroption_vector->setEnabled( option );


    if( option == true ){
        int option_color = cb_coloroption_vector->currentIndex();
        canvas_computation->selectProperty( id, option, option_color );

    }
    else
        canvas_computation->selectProperty( id, option );


}

void MainWindow::computePressure()
{
    emit computePressureProperty();
    canvas_computation->fillMenuProperties();

    statusBar()->showMessage( "Pressure computed" );
}

void MainWindow::computeVelocity()
{

    emit computeVelocityProperty();
    canvas_computation->fillMenuProperties();

    statusBar()->showMessage( "Velocity computed" );
}

void MainWindow::computeTOF()
{
    emit computeTOFProperty();
    canvas_computation->fillMenuProperties();

    statusBar()->showMessage( "TOF computed" );
}

void MainWindow::updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells )
{
    cb_compute_property->clear();

    int ndata = (int) ppoints.size();
    for( int i = 0; i < ndata; ++i )
        cb_compute_property->addItem( ppoints[ i ].c_str() );


    ndata = (int) pcells.size();
    for( int i = 0; i < ndata; ++i )
        cb_compute_property->addItem( pcells[ i ].c_str() );


}


void MainWindow::createInputDialog()
{
    dg_inputuser = new QDialog( this );
    tb_inputuser = new QToolBox();

    createWidgetInputFiles();
    createWidgetInputTolerance();

    tb_inputuser->addItem( wd_inputfiles, tr( "Input Files") );
    tb_inputuser->addItem( wd_inputtolerance, tr( "Tolerance")  );

    btns_inputdialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

    QFrame *ln_inputdialog = new QFrame();
    ln_inputdialog->setFrameStyle(QFrame::HLine | QFrame::Sunken);

    QVBoxLayout *vb_inputdialog = new QVBoxLayout();
    vb_inputdialog->addWidget( tb_inputuser );
    vb_inputdialog->addWidget( ln_inputdialog );
    vb_inputdialog->addWidget( btns_inputdialog );

    dg_inputuser->setLayout( vb_inputdialog );

    connect( btn_finduserfile, SIGNAL( pressed() ), this, SLOT( findUserFile() ));
    connect( btn_findsurfacefile, SIGNAL( pressed() ), this, SLOT( findSurfaceFile() ));
    connect( btns_inputdialog, SIGNAL( accepted() ), this, SLOT( acceptInputUser() ));
    connect( btns_inputdialog, SIGNAL( rejected() ), this, SLOT( rejectInputUser() ) );

}


void MainWindow::createWidgetInputFiles()
{
    wd_inputfiles = new QWidget();

    QLabel *lb_userfile = new QLabel( "User input: " );
    edt_userfile = new QLineEdit();
    edt_userfile->setText( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/inputs/userinput_example9.txt");
    btn_finduserfile = new QPushButton();
    btn_finduserfile->setIcon( QIcon( ":/images/icons/folder.png" ) );

    QLabel *lb_surfacefile = new QLabel( "Surface file: " );
    edt_surfacefile = new QLineEdit();
    edt_surfacefile->setText( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/inputs/boxexample9coarse.poly");
    btn_findsurfacefile = new QPushButton();
    btn_findsurfacefile->setIcon( QIcon( ":/images/icons/folder.png" ) );

    QHBoxLayout *hb_userfile = new QHBoxLayout();
    hb_userfile->addWidget( lb_userfile );
    hb_userfile->addWidget( edt_userfile );
    hb_userfile->addWidget( btn_finduserfile );

    QHBoxLayout *hb_surfacefile = new QHBoxLayout();
    hb_surfacefile->addWidget( lb_surfacefile );
    hb_surfacefile->addWidget( edt_surfacefile );
    hb_surfacefile->addWidget( btn_findsurfacefile );

    QVBoxLayout *vb_inputfiles = new QVBoxLayout();
    vb_inputfiles->addLayout( hb_userfile );
    vb_inputfiles->addLayout( hb_surfacefile );

    wd_inputfiles->setLayout( vb_inputfiles );

}

void MainWindow::createWidgetInputTolerance()
{
    wd_inputtolerance = new QWidget();

    QLabel *lb_tolerance = new QLabel( "User input: " );
    edt_tolerance1 = new QLineEdit();
    edt_tolerance1->setText( "0.000000001" );
    edt_tolerance2 = new QLineEdit();
    edt_tolerance2->setText( "0.00001" );

    QHBoxLayout *hb_toleranceinput = new QHBoxLayout();
    hb_toleranceinput->addWidget( lb_tolerance );
    hb_toleranceinput->addWidget( edt_tolerance1 );
    hb_toleranceinput->addWidget( edt_tolerance2 );

    wd_inputtolerance->setLayout( hb_toleranceinput );

}

void MainWindow::findUserFile()
{
    QString filename = QFileDialog::getOpenFileName( this );
    if( filename.isEmpty() == true ) return;

    edt_userfile->setText( filename );
}

void MainWindow::findSurfaceFile()
{
    QString filename = QFileDialog::getOpenFileName( this );
    if( filename.isEmpty() == true ) return;

    edt_surfacefile->setText( filename );


}

void MainWindow::acceptInputUser()
{
    QString filename_input = edt_userfile->text();
    QString filename_surface = edt_surfacefile->text();

    QStringList list = filename_surface.split( "\." );
    QString name_of_file = list[ 0 ];
    QString extension_of_file = list[ 1 ];

    float tol1 = edt_tolerance1->text().toFloat();
    float tol2 = edt_tolerance2->text().toFloat();

    sendInputUser( filename_input.toStdString(), name_of_file.toStdString(), tol1, tol2  );
    canvas_computation->showSurface();

    statusBar()->showMessage( "Surface loaded" );

    dg_inputuser->close();
}

void MainWindow::rejectInputUser()
{
    edt_userfile->clear();
    edt_surfacefile->clear();
    edt_tolerance1->clear();
    edt_tolerance2->clear();

    dg_inputuser->close();
}

void MainWindow::selectColorVectorOption( int idc )
{
    int id = cb_compute_property->currentIndex();
    int option_color = idc;
    canvas_computation->selectProperty( id, true, option_color );
}


void MainWindow::callComputationElements()
{
    createActionsComputation();
    createComputationModule();
    createToolbarComputation();
    createInputDialog();

}
