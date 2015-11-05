#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QStyle>

#include "MainWindow.h"



MainWindow::MainWindow( QWidget *parent) : QMainWindow( parent )
{
    createWindow();
}

MainWindow::~MainWindow()
{
//    canvas2D->clear();
}

void MainWindow::createWindow()
{
    this->setMinimumSize( 800, 600 );

    createActions();
    createMenuBar();
    createToolbar();
    create2DModule();
    create3DModule();
    createComputationModule();
    createToolbarComputation();

    createInputDialog();

    emit setColor( 0, 0, 128 );

    statusBar()->showMessage( "Status" );

}

void MainWindow::createActions()
{
    ac_new = new QAction( tr( "&New" ), this );
    ac_new->setIcon( QIcon( ":/images/icons/page_white.png" ) );

    ac_open = new QAction( tr( "&Open" ), this );
    ac_open->setIcon( QIcon( ":/images/icons/folder.png" ) );
    ac_open->setDisabled( true );

    ac_save = new QAction( tr( "&Save" ), this );
    ac_save->setIcon( QIcon( ":/images/icons/diskette.png" ) );
    ac_save->setDisabled( true );

    ac_export = new QAction( tr( "&Export" ), this );
    ac_export->setIcon( QIcon( ":/images/icons/document_export.png" ) );
    ac_export->setDisabled( true );

    ac_exit = new QAction( tr( "E&xit" ), this );
    ac_exit->setIcon( QIcon( ":/images/icons/door_out.png" ) );

    ac_compute = new QAction( tr( "&Compute" ), this );
    ac_compute->setIcon( QIcon( ":/images/icons/sum.png" ) );

    ac_contents = new QAction( tr( "Contents" ), this );
    ac_contents->setDisabled( true );

    ac_about = new QAction( tr( "&About" ), this );

    ac_removeabove = new QAction( tr( "Remove Above Intersection" ), this );;
    ac_removeabove->setIcon( QIcon( ":/images/icons/removeabove.png" ) );
    ac_removeabove->setCheckable( true );

    ac_removebelow = new QAction( tr( "Remove Below Intersection" ), this );;
    ac_removebelow->setIcon( QIcon( ":/images/icons/removebelow.png" ) );
    ac_removebelow->setCheckable( true );

    ac_select = new QAction( tr( "Select" ), this );;
    ac_select->setIcon( QIcon( ":/images/icons/pointer.png" ) );
    ac_select->setCheckable( true );

    cd_pickercolor = new QColorDialog();
    cd_pickercolor->setWindowFlags( Qt::Widget );
    cd_pickercolor->setCurrentColor( QColor( 0, 0, 128 ) );

    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );

    ac_open_surface = new QAction( tr( "Open Surface..." ), this );;
    ac_open_userinput = new QAction( tr( "Open User Input..." ), this );;
    ac_compute_volumetric  = new QAction( tr( "Volumetric Meshing..." ), this );;


   ac_compute_pressure = new QAction( tr( "Compute Pressure" ), this );
   ac_compute_velocity = new QAction( tr( "Compute Velocity" ), this );
   ac_compute_tof = new QAction( tr( "Compute TOF" ), this );


   ac_wdwsketching = new QAction( tr( "Window Sketching" ), this );
   ac_window3d = new QAction( tr( "Window 3D" ), this );
   ac_flowcomputation = new QAction( tr( "Window Flow Computation" ), this );

    connect( ac_new, SIGNAL( triggered() ), this, SLOT( newSection() ) );
    connect( ac_removeabove, SIGNAL( triggered() ), this, SLOT( applyRemoveAbove() ) );
    connect( ac_removebelow, SIGNAL( triggered() ), this, SLOT( applyRemoveBelow() ) );
    connect( ac_select, SIGNAL( triggered() ), this, SLOT( pointerSelection() ) );
    connect( ac_exit, SIGNAL( triggered() ), this, SLOT( close() ) );

    connect( ac_compute, SIGNAL( triggered() ), this, SLOT( doComputation() ) );

    connect( ac_open_surface, SIGNAL( triggered() ), this, SLOT( openSurfaceFile() ) );
    connect( ac_open_userinput, SIGNAL( triggered() ), this, SLOT( openUserInputFile() ) );
    connect( ac_compute_volumetric, SIGNAL( triggered() ), this, SLOT( createMeshVolumetric() ) );

    connect( ac_compute_pressure, SIGNAL( triggered() ), this, SLOT( computePressure() ) );
    connect( ac_compute_velocity, SIGNAL( triggered() ), this, SLOT( computeVelocity() ) );
    connect( ac_compute_tof, SIGNAL( triggered() ), this, SLOT( computeTOF() ) );

    cb_compute_property = new QComboBox();
    cb_coloroption_vector = new QComboBox();
    cb_coloroption_vector->addItem( "Magnitude" );
    cb_coloroption_vector->addItem( "X" );
    cb_coloroption_vector->addItem( "Y" );
    cb_coloroption_vector->addItem( "Z" );
    cb_coloroption_vector->setEnabled( false );

    connect( cb_compute_property, SIGNAL( currentIndexChanged( int ) ) , this, SLOT( selectProperty( int ) ) );


}


void MainWindow::createMenuBar()
{

    QMenu *mn_file = menuBar()->addMenu( tr( "&File" ) );
    mn_file->addAction( ac_new );
    mn_file->addAction( ac_open );
    mn_file->addAction( ac_save );
    mn_file->addAction( ac_export );
    mn_file->addAction( ac_exit );

    QMenu *mn_tools = menuBar()->addMenu( tr( "&Tools" ) );
    mn_tools->addAction( ac_compute );

    QMenu *mn_windows = menuBar()->addMenu( tr( "&Windows" ) );
    mn_windows->addAction( ac_wdwsketching );
    mn_windows->addAction( ac_window3d );
    mn_windows->addAction( ac_flowcomputation );
    ac_wdwsketching->setCheckable( true );
    ac_window3d->setCheckable( true );
    ac_flowcomputation->setCheckable( true );

    QMenu *mn_help = menuBar()->addMenu( tr( "&Help" ) );
    mn_help->addAction( ac_contents );
    mn_help->addAction( ac_about );


}

void MainWindow::createToolbar()
{
    tlb_section = addToolBar( tr( "Section" ) );
    tlb_section->addAction( ac_new );
    tlb_section->addAction( ac_open );
    tlb_section->addAction( ac_save );
    tlb_section->addAction( ac_export );

    tlb_computation = addToolBar( tr( "Computation" ) );
    tlb_computation->addAction( ac_compute );


    tlb_rules = addToolBar( tr( "Rules" ) );
    tlb_rules->addAction( ac_removeabove );
    tlb_rules->addAction( ac_removebelow );

    tbt_colorsketch = new QToolButton;
    tbt_colorsketch->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_colorsketch->setIcon( QIcon( ":/images/icons/border_color.png" ) );

    mn_pickercolor = new QMenu();
    mn_pickercolor->addAction( ac_sketchcolor );
    tbt_colorsketch->setMenu( mn_pickercolor );


    tlb_customization = addToolBar( tr( "Customize" ) );
    tlb_customization->addAction( ac_select );
    tlb_customization->addWidget( tbt_colorsketch );


    connect( mn_pickercolor,   SIGNAL( aboutToShow() ),   cd_pickercolor,  SLOT( show() ) );
    connect( cd_pickercolor, SIGNAL( rejected() ), mn_pickercolor, SLOT( hide() ) );
    connect( cd_pickercolor, SIGNAL( accepted() ), this, SLOT( changeColorLine() ) );

}

void MainWindow::create2DModule()
{

    dc_2DModule = new QDockWidget( this );
    dc_2DModule->setAllowedAreas( Qt::LeftDockWidgetArea );
    dc_2DModule->setWindowTitle( "2D View" );

    canvas2D = new Canvas2D( this );

    dc_2DModule->setWidget( canvas2D );
    addDockWidget( Qt::LeftDockWidgetArea, dc_2DModule );

}

void MainWindow::create3DModule()
{

    dc_3DModule = new QDockWidget( this );
    dc_3DModule->setAllowedAreas( Qt::RightDockWidgetArea );
    dc_3DModule->setWindowTitle( "3D View" );

    QGLFormat fmt;
    fmt.setVersion( 3, 0 );
    fmt.setProfile( QGLFormat::CoreProfile);


    QFrame *fr = new QFrame( this );
    fr->setFrameStyle( QFrame::Box | QFrame::Sunken );


    Canvas3D *canvas3d = new Canvas3D( fmt, fr );
    QHBoxLayout *hb_canvas3d = new QHBoxLayout( this );
    hb_canvas3d->addWidget( canvas3d );

    fr->setLayout( hb_canvas3d );
    dc_3DModule->setWidget( fr );
    addDockWidget( Qt::RightDockWidgetArea, dc_3DModule );

}

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


void MainWindow::newSection()
{
    clearCanvas2D();
//    clearCanvas3D();
    clearComputation();

    statusBar()->showMessage( "New section." );
}

void MainWindow::applyRemoveAbove()
{
    bool flag = ac_removeabove->isChecked();
    if( flag == false ) return;


    ac_removebelow->setChecked( false );
    emit applyremoveabove();

    statusBar()->showMessage( "Applying remove above intersection" );

}

void MainWindow::applyRemoveBelow()
{
    bool flag = ac_removebelow->isChecked();
    if( flag == false ) return;

    ac_removeabove->setChecked( false );
    emit applyremovebelow();

    statusBar()->showMessage( "Applying remove below intersection" );
}

void MainWindow::pointerSelection()
{
    bool flag = ac_select->isChecked();

    if( flag == false )
    {
        canvas2D->setDragMode( QGraphicsView::NoDrag );
        emit sketchingMode();
        return;
    }

    canvas2D->setDragMode( QGraphicsView::RubberBandDrag );

    emit selectMode();

}

void MainWindow::clearCanvas2D()
{
    canvas2D->clear();
//    resetToolBar();
}


void MainWindow::changeColorLine()
{
    mn_pickercolor->hide();

    QColor c = cd_pickercolor->selectedColor();
    emit setColor( c.red(), c.green(), c.blue() );

}

void MainWindow::doComputation()
{
    addDockWidget( Qt::RightDockWidgetArea, dc_computation );
    dc_computation->show();
}



void MainWindow::clearComputation()
{
    canvas_computation->resetSetup();
    canvas_computation->updateGL();

}

void MainWindow::openSurfaceFile()
{
//    QString filename = QFileDialog::getOpenFileName( this );
//    if( filename.isEmpty() == true ) return;

//    statusBar()->showMessage( "Open surface file" );

//    QStringList list = filename.split( "\." );
//    QString name_of_file = list[ 0 ];
//    QString extension_of_file = list[ 1 ];

//    emit sendSurfaceFile( name_of_file.toStdString() );

//    canvas_computation->showSurface();

//    statusBar()->showMessage( "Surface file loaded" );
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


//    if( option == true ){
//        int option_color = cb_coloroption_vector->currentIndex();
//        canvas_computation->selectProperty( id, option, option_color );

//    }
//    else
//        canvas_computation->selectProperty( id, option );


}

void MainWindow::computePressure()
{
    emit computePressureProperty();

    statusBar()->showMessage( "Pressure computed" );
}

void MainWindow::computeVelocity()
{

    emit computeVelocityProperty();

    statusBar()->showMessage( "Velocity computed" );
}

void MainWindow::computeTOF()
{
    emit computeTOFProperty();

    statusBar()->showMessage( "TOF computed" );
}

void MainWindow::updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells )
{
    cb_compute_property->clear();

    int ndata = ppoints.size();
    for( int i = 0; i < ndata; ++i )
        cb_compute_property->addItem( ppoints[ i ].c_str() );

    ndata = pcells.size();
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

    connect( ac_wdwsketching, SIGNAL( toggled(bool) ), dc_2DModule, SLOT( setVisible(bool) ) );
    connect( ac_window3d, SIGNAL( toggled(bool) ), dc_3DModule, SLOT( setVisible(bool) ) );
    connect( ac_flowcomputation, SIGNAL( toggled(bool) ), dc_computation, SLOT( setVisible(bool) ) );


}


void MainWindow::createWidgetInputFiles()
{
    wd_inputfiles = new QWidget();

    QLabel *lb_userfile = new QLabel( "User input: " );
    edt_userfile = new QLineEdit();
    btn_finduserfile = new QPushButton();

    QLabel *lb_surfacefile = new QLabel( "Surface file: " );
    edt_surfacefile = new QLineEdit();
    btn_findsurfacefile = new QPushButton();

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
    edt_tolerance2 = new QLineEdit();

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
