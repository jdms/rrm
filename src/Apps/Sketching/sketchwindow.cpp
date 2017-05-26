#include "sketchwindow.h"
#include "./MainWindow/Widgets/ColorPicker.h"
#include "csectionscene.h"
#include "topviewscene.h"

#include <QtWidgets>


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
    setCentralWidget( gv_view );
}


void SketchWindow::createScene()
{
    if(is_customizable == true )
        scene = new CSectionScene();
    else
        scene = new TopViewScene();

    gv_view->setScene( scene );
}


void SketchWindow::createToolbarActions()
{
    QAction* ac_discard_sketch = new QAction( "Discard", this );
    connect( ac_discard_sketch, &QAction::triggered, this, &SketchWindow::discardSketch );

    QAction* ac_commit_sketch = new QAction( "Commit", this );
    connect( ac_commit_sketch, &QAction::triggered, this, &SketchWindow::commitSketch );

    QAction* ac_create_surface = new QAction( "Create", this );
    connect( ac_create_surface, &QAction::triggered, this, &SketchWindow::createSurface );

    QAction* ac_edit_boundary = new QAction( "Edit Boundary", this );
    ac_edit_boundary->setVisible( is_customizable );

    ColorPicker* cp_define_color = new ColorPicker( this );
    cp_define_color->setVisible( is_customizable );

    QToolBar* tb_commands = new QToolBar( this );
    tb_commands->addAction( ac_discard_sketch );
    tb_commands->addAction( ac_commit_sketch );
    tb_commands->addAction( ac_create_surface );
    tb_commands->addSeparator();
    tb_commands->addAction( ac_edit_boundary );
    tb_commands->addWidget( cp_define_color );
    addToolBar( tb_commands );
}


SketchScene_Refactored* SketchWindow::getScene() const
{
    return scene;
}
