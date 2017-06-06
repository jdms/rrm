#include "sketchwindow.h"
#include "./MainWindow/Widgets/ColorPicker.h"
#include "csectionscene.h"
#include "topviewscene.h"

#include <QtWidgets>
#include <QWheelEvent>


SketchWindow::SketchWindow( QWidget* parent, bool customizable ): QMainWindow( parent ),
                                                                  is_customizable( customizable )
{
    createWindow();
    createScene();
    createToolbarActions();
}


void SketchWindow::createWindow()
{
    gv_view = new QGraphicsView();
    gv_view->setRenderHint( QPainter::Antialiasing, true );
    gv_view->setAcceptDrops( true );
    setCentralWidget( gv_view );
}


void SketchWindow::createScene()
{
    if(is_customizable == true ){
        scene = new CSectionScene();
        gv_view->scale( 1, -1 );
    }
    else
        scene = new TopViewScene();

    gv_view->setScene( scene );


}


void SketchWindow::createToolbarActions()
{
    QAction* ac_discard_sketch = new QAction( "Discard", this );
    connect( ac_discard_sketch, &QAction::triggered, scene, &SketchScene_Refactored::clearSketch );

    QAction* ac_commit_sketch = new QAction( "Commit", this );
    connect( ac_commit_sketch, &QAction::triggered, scene, &SketchScene_Refactored::finishSketch );

    QAction* ac_create_surface = new QAction( "Create", this );
    connect( ac_create_surface, &QAction::triggered, [=](){ emit createSurface(); } );

    ac_edit_boundary = new QAction( "Edit Boundary", this );
    ac_edit_boundary->setCheckable( true );
    ac_edit_boundary->setVisible( is_customizable );

    connect( ac_edit_boundary, &QAction::toggled, scene, &SketchScene_Refactored::setModeEditingBoundary );
    connect( scene, &SketchScene_Refactored::updateVolumeDimensions, [=](){
                                             ac_edit_boundary->setChecked( false ); } );

    QAction* ac_edit_scene = new QAction( "Edit Objects", this );
    ac_edit_scene->setCheckable( true );
    connect( ac_edit_scene, &QAction::toggled, scene, &SketchScene_Refactored::setModeEditable );

    QAction* ac_remove_curve = new QAction( "Delete Curve", this );
    ac_remove_curve->setVisible( is_customizable );
    connect( ac_remove_curve, &QAction::triggered, scene, &SketchScene_Refactored::removeCurve );
    connect( scene, &SketchScene_Refactored::enableDeleting, ac_remove_curve, &QAction::setEnabled );


    QAction* ac_remove_image = new QAction( "Remove Image", this );
    connect( ac_remove_image, &QAction::triggered, scene, &SketchScene_Refactored::removeImageFromCrossSection );


    ColorPicker* cp_define_color = new ColorPicker( this );
    cp_define_color->setVisible( is_customizable );
    connect( cp_define_color, &ColorPicker::colorSelected, [=]( const QColor& c ){ emit defineColorCurrent( c ); } );

    QToolBar* tb_commands = new QToolBar( this );
    tb_commands->addAction( ac_discard_sketch );
    tb_commands->addAction( ac_commit_sketch );
    tb_commands->addAction( ac_create_surface );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_edit_scene );
    tb_commands->addAction( ac_remove_curve );
    tb_commands->addAction( ac_remove_image );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_edit_boundary );
    tb_commands->addWidget( cp_define_color );
    addToolBar( tb_commands );

    tb_commands->setFixedHeight( 27 );
}



void SketchWindow::setEnabledVolumeResize( bool status )
{
    ac_edit_boundary->setEnabled( status );
}


SketchScene_Refactored* SketchWindow::getScene() const
{
    return scene;
}


void SketchWindow::wheelEvent( QWheelEvent *event )
{
    if( event->delta() > 0 )
        gv_view->scale( ZOOM_SCALE, ZOOM_SCALE );
    else
        gv_view->scale( 1.0/ZOOM_SCALE, 1.0/ZOOM_SCALE );


    gv_view->ensureVisible( scene->sceneRect() );
    gv_view->update();

    QMainWindow::wheelEvent( event );
}
