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

    emit setColor( 0, 0, 128 );

    statusBar()->showMessage( "Status" );

}

void MainWindow::createActions()
{
    ac_new = new QAction( tr( "&New" ), this );
    ac_new->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/page_white.png" ) );

    ac_open = new QAction( tr( "&Open" ), this );
    ac_open->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/folder.png" ) );
    ac_open->setDisabled( true );

    ac_save = new QAction( tr( "&Save" ), this );
    ac_save->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/diskette.png" ) );
    ac_save->setDisabled( true );

    ac_export = new QAction( tr( "&Export" ), this );
    ac_export->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/document_export.png" ) );
    ac_export->setDisabled( true );

    ac_exit = new QAction( tr( "E&xit" ), this );
    ac_exit->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/door_out.png" ) );

    ac_compute = new QAction( tr( "&Compute" ), this );
    ac_compute->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/sum.png" ) );

    ac_contents = new QAction( tr( "Contents" ), this );
    ac_contents->setDisabled( true );

    ac_about = new QAction( tr( "&About" ), this );

    ac_removeabove = new QAction( tr( "Remove Above" ), this );;
    ac_removeabove->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/removeabove.png" ) );
    ac_removeabove->setCheckable( true );

    ac_removebelow = new QAction( tr( "Remove Below" ), this );;
    ac_removebelow->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/removebelow.png" ) );
    ac_removebelow->setCheckable( true );

    ac_select = new QAction( tr( "Select" ), this );;
    ac_select->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/pointer.png" ) );
    ac_select->setCheckable( true );

    cd_pickercolor = new QColorDialog();
    cd_pickercolor->setWindowFlags( Qt::Widget );
    cd_pickercolor->setCurrentColor( QColor( 0, 0, 128 ) );

    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );

    ac_open_surface  = new QAction( tr( "Open Surface..." ), this );;
    ac_compute_volumetric  = new QAction( tr( "Compute Volumetric..." ), this );;


   ac_compute_pressure = new QAction( tr( "Compute Pressure" ), this );;
   ac_compute_velocity = new QAction( tr( "Compute Velocity" ), this );;
   ac_compute_tof = new QAction( tr( "Compute TOF" ), this );;


    connect( ac_new, SIGNAL( triggered() ), this, SLOT( newSection() ) );
    connect( ac_removeabove, SIGNAL( triggered() ), this, SLOT( applyRemoveAbove() ) );
    connect( ac_removebelow, SIGNAL( triggered() ), this, SLOT( applyRemoveBelow() ) );
    connect( ac_select, SIGNAL( triggered() ), this, SLOT( pointerSelection() ) );
    connect( ac_exit, SIGNAL( triggered() ), this, SLOT( close() ) );

    connect( ac_compute, SIGNAL( triggered() ), this, SLOT( doComputation() ) );

    connect( ac_open_surface, SIGNAL( triggered() ), this, SLOT( openSurfaceFile() ) );
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
    tbt_colorsketch->setIcon( QIcon( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/border_color.png" ) );

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

}

void MainWindow::createToolbarComputation()
{

    QLabel *lb_name_property = new QLabel( tr( "Property" ) );

    tlb_workflow_flow = addToolBar( tr( "Workflow" ) );
    tlb_workflow_flow->addAction( ac_open_surface );
    tlb_workflow_flow->addAction( ac_compute_volumetric );
    tlb_workflow_flow->addAction( ac_compute_pressure );
    tlb_workflow_flow->addAction( ac_compute_velocity );
    tlb_workflow_flow->addAction( ac_compute_tof );
    tlb_workflow_flow->addWidget( lb_name_property );
    tlb_workflow_flow->addWidget( cb_compute_property );
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

}

void MainWindow::applyRemoveBelow()
{
    bool flag = ac_removebelow->isChecked();
    if( flag == false ) return;

    ac_removeabove->setChecked( false );
    emit applyremovebelow();
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
    canvas_computation->showData();
}



void MainWindow::clearComputation()
{
    canvas_computation->resetSetup();
    canvas_computation->updateGL();

}

void MainWindow::openSurfaceFile()
{
    QString filename = QFileDialog::getOpenFileName( this );
    if( filename.isEmpty() == true ) return;


    QStringList list = filename.split( "\." );
    QString name_of_file = list[ 0 ];
    QString extension_of_file = list[ 1 ];


//    cout << name_of_file.toStdString().c_str() << endl;
//    cout << extension_of_file.toStdString().c_str() << endl;

    emit sendSurfaceFile( name_of_file.toStdString() );
}

void MainWindow::createMeshVolumetric()
{
    emit computeVolume();
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
}

void MainWindow::computeVelocity()
{

    emit computeVelocityProperty();
}

void MainWindow::computeTOF()
{
    emit computeTOFProperty();
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
