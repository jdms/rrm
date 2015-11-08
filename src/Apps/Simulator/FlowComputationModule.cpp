#include "FlowComputationModule.h"

FlowComputationModule::FlowComputationModule()
{
    dg_inputuser = new DialogInputFlowComputation( this );


    createWindow();
    createActions();
    createToolbar();

}

void FlowComputationModule::createActions()
{

    ac_compute = new QAction( tr( "&Compute" ), this );
    ac_compute->setIcon( QIcon( ":/images/icons/sum.png" ) );
    connect( ac_compute, SIGNAL( triggered() ), this, SLOT( doComputation() ) );


    ac_flowcomputation = new QAction( tr( "Window Flow Computation" ), this );
    ac_flowcomputation->setCheckable( true );


    ac_open_surface = new QAction( tr( "Open Surface..." ), this );;
    ac_open_userinput = new QAction( tr( "Open User Input..." ), this );;
    ac_compute_volumetric  = new QAction( tr( "Volumetric Meshing..." ), this );;


    ac_compute_pressure = new QAction( tr( "Compute Pressure" ), this );
    ac_compute_velocity = new QAction( tr( "Compute Velocity" ), this );
    ac_compute_tof = new QAction( tr( "Compute TOF" ), this );


    connect( ac_open_userinput, SIGNAL( triggered() ), dg_inputuser, SLOT( show() ) );
    connect( ac_compute_volumetric, SIGNAL( triggered() ), this, SLOT( createMeshVolumetric() ) );
    connect( ac_compute_pressure, SIGNAL( triggered() ), this, SLOT( computePressure() ) );
    connect( ac_compute_velocity, SIGNAL( triggered() ), this, SLOT( computeVelocity() ) );
    connect( ac_compute_tof, SIGNAL( triggered() ), this, SLOT( computeTOF() ) );

    connect( cb_compute_property, SIGNAL( currentIndexChanged( int ) ) , this, SLOT( selectProperty( int ) ) );
    connect( cb_coloroption_vector, SIGNAL( currentIndexChanged( int ) ) , this, SLOT( selectColorVectorOption( int ) ) );



}


void FlowComputationModule::createWindow()
{
    this->setAllowedAreas( Qt::RightDockWidgetArea );
    this->setWindowTitle( "Flow Visualization" );

    QFrame *fr = new QFrame( this );
    fr->setFrameStyle( QFrame::Box | QFrame::Sunken );

    canvas_computation = new CanvasComputation( this );
    canvas_computation->setMinimumHeight( 350 );

    QHBoxLayout *hb_canvascomputation = new QHBoxLayout( this );
    hb_canvascomputation->addWidget( canvas_computation );

    fr->setLayout( hb_canvascomputation );

    mw_canvas_computation = new QMainWindow();
    mw_canvas_computation->setCentralWidget( fr );

    cb_compute_property = new QComboBox();
    cb_compute_property->setMinimumWidth( 120 );

    cb_coloroption_vector = new QComboBox();
    cb_coloroption_vector->addItem( "Magnitude" );
    cb_coloroption_vector->addItem( "X" );
    cb_coloroption_vector->addItem( "Y" );
    cb_coloroption_vector->addItem( "Z" );
    cb_coloroption_vector->setEnabled( false );

    this->setWidget( mw_canvas_computation );
    this->setVisible( false );
}


void FlowComputationModule::createToolbar()
{

    QLabel *lb_name_property = new QLabel( tr( "  Property  " ) );
    QLabel *lb_space = new QLabel("  ");

    tlb_workflow_flow = mw_canvas_computation->addToolBar( tr( "Workflow" ) );

    tlb_workflow_flow->addAction( ac_open_userinput );
    tlb_workflow_flow->addSeparator();

    tlb_workflow_flow->addAction( ac_compute_volumetric );
    tlb_workflow_flow->addAction( ac_compute_pressure );
    tlb_workflow_flow->addAction( ac_compute_velocity );
    tlb_workflow_flow->addAction( ac_compute_tof );

    tlb_workflow_flow->addSeparator();

    tlb_workflow_flow->addWidget( lb_name_property );
    tlb_workflow_flow->addWidget( cb_compute_property );
    tlb_workflow_flow->addWidget( lb_space );
    tlb_workflow_flow->addWidget( cb_coloroption_vector );



//    mw_canvas_computation->addToolBar( tlb_workflow_flow );

}



void FlowComputationModule::createMeshVolumetric()
{
    emit computeVolume();
    canvas_computation->showVolumetricGrid();
}


void FlowComputationModule::computePressure()
{
    emit computePressureProperty();
}


void FlowComputationModule::computeVelocity()
{
    emit computeVelocityProperty();
    canvas_computation->fillMenuProperties();
}


void FlowComputationModule::computeTOF()
{
    emit computeTOFProperty();
    canvas_computation->fillMenuProperties();

}


void FlowComputationModule::selectProperty( int id )
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


void FlowComputationModule::acceptInputUser()
{
     QString userfile, surfacefile;
     float tol1, tol2;

    dg_inputuser->getUserInputs( userfile, surfacefile, tol1, tol2 );

    QStringList list = surfacefile.split( "\." );
    QString name_of_file = list[ 0 ];
    QString extension_of_file = list[ 1 ];

    sendInputUser( userfile.toStdString(), name_of_file.toStdString(), tol1, tol2  );
    canvas_computation->showSurface();
    dg_inputuser->close();

}


void FlowComputationModule::selectColorVectorOption( int idc )
{

    int id = cb_compute_property->currentIndex();
    int option_color = idc;
    canvas_computation->selectProperty( id, true, option_color );
}


void FlowComputationModule::updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells )
{
    cb_compute_property->clear();

    int ndata = (int) ppoints.size();
    for( int i = 0; i < ndata; ++i )
        cb_compute_property->addItem( ppoints[ i ].c_str() );


    ndata = (int) pcells.size();
    for( int i = 0; i < ndata; ++i )
        cb_compute_property->addItem( pcells[ i ].c_str() );


}


