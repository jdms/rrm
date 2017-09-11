#include "sketch_window.h"
#include "./mainwindow/widgets/color_picker.h"
#include "csection_scene.h"
#include "topview_scene.h"

#include <QtWidgets>
#include <QWheelEvent>
#include <QFileDialog>


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
    connect( ac_discard_sketch, &QAction::triggered, scene, &SketchScene::clearSketch );

    QAction* ac_commit_sketch = new QAction( "Commit", this );
    connect( ac_commit_sketch, &QAction::triggered, scene, &SketchScene::finishSketch );

    QAction* ac_create_surface = new QAction( "Create", this );
    connect( ac_create_surface, &QAction::triggered, [=](){ emit createSurface(); } );
    connect( scene, &SketchScene::createSurface, ac_create_surface, &QAction::trigger );

    ac_edit_boundary = new QAction( "Edit Boundary", this );
    ac_edit_boundary->setCheckable( true );
    ac_edit_boundary->setVisible( is_customizable );

    connect( ac_edit_boundary, &QAction::toggled, scene, &SketchScene::setModeEditingBoundary );
    connect( scene, &SketchScene::updateVolumeDimensions, [=](){
                                             ac_edit_boundary->setChecked( false ); } );

    QAction* ac_edit_scene = new QAction( "Edit Objects", this );
    ac_edit_scene->setCheckable( true );
    ac_edit_scene->setVisible( is_customizable );
    connect( ac_edit_scene, &QAction::toggled, scene, &SketchScene::setModeEditable );

    QAction* ac_remove_curve = new QAction( "Delete Curve", this );
    ac_remove_curve->setVisible( is_customizable );
    connect( ac_remove_curve, &QAction::triggered, scene, &SketchScene::removeCurve );
    connect( scene, &SketchScene::enableDeleting, ac_remove_curve, &QAction::setEnabled );



    QAction* ac_resize_image = new QAction( "Resize Image", this );
    ac_resize_image->setCheckable( true );
    connect( ac_resize_image, &QAction::toggled, scene, &SketchScene::setModeResizingImage );
    connect( scene, &SketchScene::enableResizingImage, ac_resize_image, &QAction::setEnabled );
    connect( scene, &SketchScene::enableResizingImage, [=]( bool status ) { if( status == true ) return;
                                                                             ac_resize_image->setChecked( false ); });


    QAction* ac_remove_image = new QAction( "Remove Image", this );
    connect( ac_remove_image, &QAction::triggered, scene, &SketchScene::removeImageFromCrossSection );


    ColorPicker* cp_define_color = new ColorPicker( this );
    cp_define_color->setVisible( is_customizable );
    connect( cp_define_color, &ColorPicker::colorSelected, [=]( const QColor& c ){ emit defineColorCurrent( c ); } );


    connect( scene, &SketchScene::updateColor, [=]( const QColor& c ){
                                                            cp_define_color->setColor( c ); } );

    connect( scene, &SketchScene::setUpColor, [=](){
                                                            cp_define_color->defineRandomColor(); } );


    QAction* ac_screenshot = new QAction( "Screenshot", this );
    connect( ac_screenshot, &QAction::triggered, this, &SketchWindow::screenshot );

    QAction* ac_axes = new QAction( "Axes", this );
    ac_axes->setCheckable( true );
    ac_axes->setChecked( true );
    connect( ac_axes, &QAction::triggered, [=]( bool status ){
                                        scene->axes.setVisible( status );
                                        scene->update(); });



    QToolBar* tb_commands = new QToolBar( this );
    tb_commands->addWidget( cp_define_color );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_edit_boundary );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_discard_sketch );
    tb_commands->addAction( ac_commit_sketch );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_edit_scene );
    tb_commands->addAction( ac_remove_curve );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_create_surface );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_resize_image );
    tb_commands->addAction( ac_remove_image );
    tb_commands->addAction( ac_screenshot );
    tb_commands->addAction( ac_axes );
    addToolBar( tb_commands );

    tb_commands->setFixedHeight( 34 );




    ac_discard_sketch->setIcon( QIcon( ":/images/icons/denyCurve.png" ) );
    ac_commit_sketch->setIcon( QIcon( ":/images/icons/add_curve2.png" ) );
    ac_create_surface->setIcon( QIcon( ":/images/icons/accept.png" ) );


    ac_edit_scene->setIcon( QIcon( ":/images/icons/select_curve.png" ) );
    ac_remove_curve->setIcon( QIcon( ":/images/icons/delete_curve.png" ) );


    ac_resize_image->setIcon( QIcon( ":/images/icons/resize_image.png" ) );
    ac_remove_image->setIcon( QIcon( ":/images/icons/remove_picture.png" ) );
    ac_edit_boundary->setIcon( QIcon( ":/images/icons/newBoundary.png" ) );

    ac_screenshot->setIcon( QIcon( ":/images/icons/Camera.png" ) );
    ac_axes->setIcon( QIcon( ":/images/icons/axes.png" ) );
}



void SketchWindow::setEnabledVolumeResize( bool status )
{
    ac_edit_boundary->setEnabled( status );
}


SketchScene* SketchWindow::getScene() const
{
    return scene;
}


void SketchWindow::clear()
{
    ac_edit_boundary->setEnabled( true );
}



void SketchWindow::screenshot()
{
    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
                                                         &selectedFilter );

    if( selectedFilter == "PNG (*.png)" )
    {
        scene->savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        scene->savetoVectorImage( name_of_file );
    }
}



void SketchWindow::wheelEvent( QWheelEvent *event )
{

    if( event->modifiers() & Qt::ControlModifier )
    {
        if ( event->delta() > 0 )
        {
            emit decreaseSlider();
        }

        else if ( event->delta() < 0 )
        {
            emit increaseSlider();
        }
    }
    else
    {

        if( event->delta() > 0 )
            gv_view->scale( ZOOM_SCALE, ZOOM_SCALE );
        else
            gv_view->scale( 1.0/ZOOM_SCALE, 1.0/ZOOM_SCALE );


        gv_view->ensureVisible( &(scene->axes), 100, 100 );
        gv_view->update();
    }

    QMainWindow::wheelEvent( event );
}

