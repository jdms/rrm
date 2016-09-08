#include "View3DWindow.hpp"

View3DWindow::View3DWindow ( QWidget* parent ) : QMainWindow(parent)
{

    createWindow();

    connect( canvas3d, &View3DCanvas::openglInitialized, [=](){ emit initializeScene(); } );

}


void View3DWindow::createWindow()
{

    this->setWindowTitle ( "3D View" );


    hs_curve_resolution = new QSlider( Qt::Horizontal );
    hs_curve_resolution->setRange( 1, 10 );


    hs_surface_resolution = new QSlider( Qt::Horizontal );
    hs_surface_resolution->setRange( 1, 10 );


    hs_extrusion = new QSlider( Qt::Horizontal );
    hs_extrusion->setRange( 10, 1000 );
    hs_extrusion->setSingleStep( 1 );
    hs_extrusion->setValue( 500 );


    QLabel* lb_curve  = new QLabel( "Curve" );
    lb_curve->setObjectName(QStringLiteral("lb_curve"));

    QLabel* lb_surface = new QLabel( "Surface" );
    lb_surface->setObjectName(QStringLiteral("lb_surface"));

    QLabel* lb_extrusion = new QLabel( "Extrusion" );
    lb_extrusion->setObjectName(QStringLiteral("lb_extrusion"));


    QFormLayout* fl_resolution_sliders = new QFormLayout();
    fl_resolution_sliders->setObjectName( QStringLiteral( "fl_resolution_sliders"  ));


    fl_resolution_sliders->setWidget( 0, QFormLayout::LabelRole, lb_curve );
    fl_resolution_sliders->setWidget( 1, QFormLayout::LabelRole, lb_surface );
    fl_resolution_sliders->setWidget( 2, QFormLayout::LabelRole, lb_extrusion );

    fl_resolution_sliders->setWidget( 0, QFormLayout::FieldRole, hs_curve_resolution );
    fl_resolution_sliders->setWidget( 1, QFormLayout::FieldRole, hs_surface_resolution );
    fl_resolution_sliders->setWidget( 2, QFormLayout::FieldRole, hs_extrusion );


    QGroupBox* gb_resolution_sliders = new QGroupBox( "Resolutions" );
    gb_resolution_sliders->setLayout( fl_resolution_sliders );
    gb_resolution_sliders->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    gb_resolution_sliders->setVisible(true);


    canvas3d = new View3DCanvas ( this );


    QVBoxLayout* hb_canvas3d = new QVBoxLayout();
    hb_canvas3d->addWidget( gb_resolution_sliders );
    hb_canvas3d->addWidget( canvas3d );



    QFrame *fr = new QFrame ( this );
    fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );
    fr->setLayout ( hb_canvas3d );


    setCentralWidget( fr );


}


void View3DWindow::setBlackScreenVisibility( bool _is_visible )
{
	groupBox->setVisible(_is_visible);
}


