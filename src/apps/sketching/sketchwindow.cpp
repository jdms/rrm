#include <QToolBar>
#include <QFileDialog>

#include "sketchwindow.h"


SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{
    createWindow();
    createToolBar();
}



void SketchWindow::createToolBar()
{

    QToolBar *tb_actions = new QToolBar();


    cp_color = new ColorPicker( this );
    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit defineColorCurrent( color_ ); } );

    ac_discard = new QAction( "Discard", this );
    ac_commit = new QAction( "Commit", this );
    ac_create = new QAction( "Create", this );

    ac_edit_boundary = new QAction( "Edit Boundary", this );
    ac_edit_boundary->setCheckable( true );

    ac_edit_scene = new QAction( "Edit Scene", this );
    ac_edit_scene->setCheckable( true );

    ac_screenshot = new QAction( "Screenshot", this );
    connect( ac_screenshot, &QAction::triggered, this, &SketchWindow::screenshot );


    ac_axes = new QAction( "Axes", this );
    ac_axes->setCheckable( true );
    ac_axes->setChecked( true );

    ac_height_map = new QAction( "Map", this );
    connect( ac_height_map, &QAction::triggered, [=](){ if( tv_main != nullptr ) std::cout << "tv_main->devicePixelRatio() = " << tv_main->devicePixelRatio() << std::endl << std::flush;
                                                                                              emit getHeightMap(); } );

    ac_fixed_csections = new QAction( "Fixed Cross-Sections", this );
    ac_fixed_csections->setCheckable( true );
    connect( ac_fixed_csections, &QAction::triggered, dw_canvas_stack, &QDockWidget::setVisible );



    tb_actions->addWidget( cp_color );
    tb_actions->addSeparator();
    tb_actions->addAction( ac_discard );
    tb_actions->addAction( ac_commit );
    tb_actions->addAction( ac_create );
    tb_actions->addSeparator();
    tb_actions->addAction( ac_edit_boundary );
    tb_actions->addAction( ac_edit_scene );
    tb_actions->addSeparator();
    tb_actions->addAction( ac_screenshot );
    tb_actions->addAction( ac_fixed_csections );
    tb_actions->addAction( ac_axes );
    tb_actions->addAction( ac_height_map );


    addToolBar( tb_actions );

}


void SketchWindow::createWindow()
{
    cs = new CanvasStack();
    dw_canvas_stack = new QDockWidget( "Fixed Cross-Sections" );
    dw_canvas_stack->setAllowedAreas( Qt::BottomDockWidgetArea );
    dw_canvas_stack->setWidget( cs );
    dw_canvas_stack->setVisible( false );
    dw_canvas_stack->setMinimumSize( WIDTH_APP, HEIGHT_APP );

    connect( cs, &CanvasStack::closeSubWindow, this, &SketchWindow::removeFixedCrossSectionCanvas );

    hb_central_widget = new QHBoxLayout( this );

    QWidget* central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );

}




void SketchWindow::setupScene( SketchScene* scene_ )
{

    if( scene_ == nullptr ) return;

    QColor color_ = cp_color->currentColor();
    scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() );



    connect( ac_discard, &QAction::triggered, [=](){ if( scene_ == nullptr) return; emit scene_->discard(); } );

    connect( ac_commit, &QAction::triggered, [=](){ if( scene_ == nullptr) return; emit scene_->commit(); } );

    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( scene_ == nullptr) return; scene_->edit( status_ ); } );

    connect( ac_edit_boundary, &QAction::triggered, [=](){ if( scene_ == nullptr) return; scene_->setModeEditingBoundary(); } );

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
                                                    { if( scene_ == nullptr) return; scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

    connect( ac_axes, &QAction::triggered, scene_, &SketchScene::setAxesVisible );



    connect( scene_, &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

    connect( scene_, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

    connect( scene_, &SketchScene::commitObject, [=](){ emit commitObject(); } );

    connect( scene_, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

    connect( scene_, &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
                                                            { emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ ); } );


    connect( scene_, &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
                                                            { emit removeCurveFromObject( depth_, index_ ); } );


    connect( scene_, &SketchScene::removeImageFromCrossSection, [=]( double depth_ )
                                                            { emit removeImageFromCrossSection( depth_ ); } );


}


void SketchWindow::addMainCanvas( CrossSection* const& cs_ )
{

    if( cs_ == nullptr ) return;

    main_scene = new SketchScene( cs_ );
    main_scene->setCurrent( true );

    main = new QGraphicsView();
    main->scale( 1, -1 );
    main->setScene( main_scene );

    main->fitInView( main_scene->itemsBoundingRect(), Qt::KeepAspectRatio);


    ////// teste

    QColor color_ = cp_color->currentColor();
    main_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() );

    connect( ac_discard, &QAction::triggered, [=](){ if( main_scene == nullptr) return; emit main_scene->discard(); } );

    connect( ac_commit, &QAction::triggered, [=](){ if( main_scene == nullptr) return; emit main_scene->commit(); } );

    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( main_scene == nullptr) return; main_scene->edit( status_ ); } );

    connect( ac_edit_boundary, &QAction::triggered, [=]( bool status_ ){ if( main_scene == nullptr) return;
                                                    if( status_ == true ) main_scene->setModeEditingBoundary();
                                                    else main_scene->setModeSketching(); } );

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
                                                    { if( main_scene == nullptr) return; main_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

    connect( ac_axes, &QAction::triggered, main_scene, &SketchScene::setAxesVisible );



    connect( main_scene, &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

    connect( main_scene, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

    connect( main_scene, &SketchScene::commitObject, [=](){ emit commitObject(); } );

    connect( main_scene, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

    connect( main_scene, &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
                                                            { emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ ); updateCanvas(); }  );


    connect( main_scene, &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
                                                            { emit removeCurveFromObject( depth_, index_ ); } );


    connect( main_scene, &SketchScene::removeImageFromCrossSection, [=]( double depth_ )
                                                            { emit removeImageFromCrossSection( depth_ ); } );

    ////// teste

    ac_height_map->setVisible( false );
    hb_central_widget->insertWidget( 0, main );

}


void SketchWindow::addTopViewCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;

    tv_scene = new SketchScene( cs_ );
    tv_scene->setCurrent( true );

    tv_main = new QGraphicsView();
    tv_main->setScene( tv_scene );
    tv_main->fitInView( tv_scene->itemsBoundingRect(), Qt::KeepAspectRatio);


    ////// teste

    QColor color_ = cp_color->currentColor();
    tv_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() );

    connect( ac_discard, &QAction::triggered, [=](){ if( tv_scene == nullptr) return; emit tv_scene->discard(); } );

    connect( ac_commit, &QAction::triggered, [=](){ if( tv_scene == nullptr) return; emit tv_scene->commit(); } );

    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( tv_scene == nullptr) return; tv_scene->edit( status_ ); } );

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
    { if( tv_scene == nullptr) return; tv_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

    connect( ac_axes, &QAction::triggered, tv_scene, &SketchScene::setAxesVisible );



    connect( tv_scene, &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

    connect( tv_scene, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

    connect( tv_scene, &SketchScene::commitObject, [=](){ emit commitObject(); } );

    connect( tv_scene, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

    connect( tv_scene, &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
    { emit setImageToTopView( file_, ox_, oy_, x_, y_ ); } );


    connect( tv_scene, &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
    { emit removeCurveFromObject( depth_, index_ ); } );


    connect( tv_scene, &SketchScene::removeImageFromCrossSection, [=](){ emit removeImageFromTopView(); } );

    ////// teste

    ac_fixed_csections->setVisible( false );
    ac_edit_boundary->setVisible( false );
    hb_central_widget->insertWidget( 0, tv_main );

}



void SketchWindow::updateCanvas()
{

    if( main != nullptr )
    {

        if( main_scene != nullptr)
        {
            main_scene->updateVolume();
            main_scene->updateCrossSection();
            main->fitInView( main_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        }

    }

    if( tv_main != nullptr )
    {

        if( tv_scene != nullptr)
        {
            tv_scene->updateVolume();
            tv_scene->updateCrossSection();
            tv_main->fitInView( tv_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        }

    }

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = cs->getElement( it->first );
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateVolume();
        sc_->updateCrossSection();
        gview_->fitInView( sc_->itemsBoundingRect(), Qt::KeepAspectRatio);
    }

}




void SketchWindow::addCrossSection( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;

    if( tv_scene == nullptr) return;
    tv_scene->addCrossSection( cs_ );

}




void SketchWindow::addObject( Object* const& obj_ )
{

    if( main == nullptr ) return;
    if( main_scene == nullptr) return;

    main_scene->addObject( obj_ );
    main->fitInView( main_scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = cs->getElement( it->first );
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->addObject( obj_ );
        gview_->fitInView( sc_->itemsBoundingRect(), Qt::KeepAspectRatio);
    }

}


void SketchWindow::updateObject( const std::size_t& index_ )
{

    if( main == nullptr ) return;
    if( main_scene == nullptr) return;

    main_scene->updateObject( index_ );
    main->fitInView( main_scene->itemsBoundingRect(), Qt::KeepAspectRatio);


    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = cs->getElement( it->first );
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateObject( index_ );
        gview_->fitInView( sc_->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}




void SketchWindow::addTrajectory( Object* const& obj_ )
{

    if( tv_scene == nullptr ) return;
    tv_scene->addTrajectory( obj_ );
    tv_main->fitInView( tv_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void SketchWindow::updateTrajectory( const std::size_t& index_ )
{
    if( tv_scene == nullptr ) return;
    tv_scene->updateTrajectory( index_ );
    tv_main->fitInView( tv_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}





void SketchWindow::setModeSelecting()
{
    if( main == nullptr ) return;
    if( main_scene == nullptr) return;

    main_scene->setModeSelecting();

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = cs->getElement( it->first );
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setModeSelecting();
    }
}


void SketchWindow::setModeSketching()
{
    if( main == nullptr ) return;
    if( main_scene == nullptr) return;

    main_scene->setModeSketching();

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = cs->getElement( it->first );
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setModeSketching();
    }

}



void SketchWindow::setCurrentColor( int r_, int g_, int b_ )
{
    cp_color->setColor( QColor( r_, g_, b_ ) );

    if( main != nullptr )
    {
        if( main_scene != nullptr)
            main_scene->setCurrentColor( r_, g_, b_ );

    }
    if( tv_main != nullptr )
    {
        if( tv_scene != nullptr )
            tv_scene->setCurrentColor( r_, g_, b_ );

    }

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = cs->getElement( it->first );
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setCurrentColor( r_, g_, b_ );
    }

    updateCanvas();
}





void SketchWindow::clear()
{
    if( main != nullptr )
    {
        if( main_scene != nullptr )
        {
            main_scene->clear();
            delete main_scene;
            main_scene = nullptr;
        }
        delete main;
        main = nullptr;
    }


    if( tv_main != nullptr )
    {
        if( tv_scene != nullptr )
        {
            tv_scene->clear();
            delete tv_scene;
            tv_scene = nullptr;
        }
        delete tv_main;
        tv_main = nullptr;


    }


    std::map< double, SketchScene* >::iterator it_ =  scenes.begin();
    while( it_ != scenes.end() )
    {
        if( it_->second == nullptr )
            scenes.erase( it_->first );
        else
            removeFixedCrossSectionCanvas( it_->first );

        it_ =  scenes.begin();
    }
    scenes.clear();


    CanvasContainer::Iterator it =  cs->begin();
    while( it != cs->end() )
    {
        cs->removeElement( it->first );
        it =  cs->begin();
    }
    cs->clear();


}




void SketchWindow::addFixedCrossSectionCanvas( CrossSection* const& cs_, QColor c_ )
{
    if( cs_ == nullptr ) return;

    dw_canvas_stack->setVisible( true );

    scenes[ cs_->getDepth() ] = new SketchScene( cs_ );
    SketchScene* const& scene_ = scenes[ cs_->getDepth() ];


    QColor color_ = cp_color->currentColor();
    scene_->addLabel( cs_->getDepth(), c_ );
    scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() );
    scene_->setCurrent( true );

    QGraphicsView* gv_ = new QGraphicsView();
    gv_->scale( 1, -1 );
    gv_->setScene( scene_ );
    gv_->setWindowTitle( QString( "CrossSection %1" ).arg( cs_->getDepth() ) );
    cs->addElement( cs_->getDepth(), gv_ );



    gv_->fitInView( scene_->itemsBoundingRect(), Qt::KeepAspectRatio);



    connect( ac_discard, &QAction::triggered, [=](){ if( scenes[ cs_->getDepth() ] == nullptr) return; emit scenes[ cs_->getDepth() ]->discard(); } );

    connect( ac_commit, &QAction::triggered, [=](){ if( scenes[ cs_->getDepth() ] == nullptr) return; emit scenes[ cs_->getDepth() ]->commit(); } );

    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( scenes[ cs_->getDepth() ] == nullptr) return; scenes[ cs_->getDepth() ]->edit( status_ ); } );

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
                                                    { if( scenes[ cs_->getDepth() ] == nullptr) return; scenes[ cs_->getDepth() ]->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

    connect( ac_axes, &QAction::triggered, scene_, &SketchScene::setAxesVisible );



    connect( scenes[ cs_->getDepth() ], &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

    connect( scenes[ cs_->getDepth() ], &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

    connect( scenes[ cs_->getDepth() ], &SketchScene::commitObject, [=](){ emit commitObject(); } );

    connect( scenes[ cs_->getDepth() ], &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

    connect( scenes[ cs_->getDepth() ], &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
                                                            { emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ ); main_scene->updateCrossSection(); } );


    connect( scenes[ cs_->getDepth() ], &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
                                                            { emit removeCurveFromObject( depth_, index_ ); } );


    connect( scenes[ cs_->getDepth() ], &SketchScene::removeImageFromCrossSection, [=]( double depth_ )
                                                            { emit removeImageFromCrossSection( depth_ ); } );


}


bool SketchWindow::removeFixedCrossSectionCanvas( double depth_ )
{

    if( scenes.find( depth_ ) != scenes.end() )
    {
        scenes[ depth_ ]->clear();
        delete scenes[ depth_ ];
        scenes.erase( depth_ );
    }

    if( cs->findElement( depth_  ) == false ) return false;
    cs->removeElement( depth_ );

    emit removeFixedCrossSection( depth_ );
    return true;

}


void SketchWindow::setFixedCrossSectionsVisible( bool status_ )
{
    dw_canvas_stack->setVisible( true );
}


void SketchWindow::setCurrentCrossSection( double depth_ )
{
    if( tv_scene != nullptr )
    {
        tv_scene->moveCurrentCrossSection( depth_ );
    }

    if( cs->findElement( depth_ ) == false ) return;
    cs->setCurrent( depth_ );
}



void SketchWindow::setTopViewImage( const std::string& image_ )
{
    if( tv_main == nullptr ) return;

    if( tv_scene != nullptr )
    {
        tv_scene->setImageToCrossSection( QString( image_.c_str() ) );
    }

    update();
}



void SketchWindow::setCrossSectionImage( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
{

    emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ );


    if( main_scene == nullptr ) return;
    main_scene->updateCrossSection();
    main->fitInView( main_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void SketchWindow::screenshot()
{
    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
                                                         &selectedFilter );


    if( main_scene != nullptr)
    {
        if( selectedFilter == "PNG (*.png)" )
        {
            main_scene->savetoRasterImage( name_of_file );
        }
        else if ( selectedFilter == "SVG (*.svg)" )
        {
            main_scene->savetoVectorImage( name_of_file );
        }
    }
    else if( tv_scene != nullptr)
    {
        if( selectedFilter == "PNG (*.png)" )
        {
            tv_scene->savetoRasterImage( name_of_file );
        }
        else if ( selectedFilter == "SVG (*.svg)" )
        {
            tv_scene->savetoVectorImage( name_of_file );
        }
    }

}
