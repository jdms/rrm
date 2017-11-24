#include "rrmapplication.h"
#include "mainwindow.h"

RRMApplication::RRMApplication(MainWindow *mw_ ): mainwindow( mw_ )
{
    mainwindow->controller = new Controller();

}


void RRMApplication::init()
{

    Scene3d* scene3d = mainwindow->canvas3d->getScene();
    mainwindow->controller->setScene3d( scene3d );
    mainwindow->controller->setObjectTree( mainwindow->object_tree );
    mainwindow->controller->init();

    setRRMDefaultValuesOnInterface();

    mainwindow->controller->addMainCrossSection( DEFAULT_CROSSSECTION_DIRECTION, DEFAULT_CROSSSECTION_DEPTHZ );
    mainwindow->controller->addTopViewCrossSection( DEFAULT_CROSSSECTION_DEPTHY );


}


void RRMApplication::setRRMDefaultValuesOnInterface()
{
    setVolumeDimensionsToController( VOLUME_WIDTH, VOLUME_HEIGHT, VOLUME_LENGTH );


}




void RRMApplication::setVolumeDimensionsToController( double width_, double height_, double length_ )
{
    mainwindow->controller->setVolumeDimensions( width_, height_, length_ );
    getVolumeDimensionsFromController();
}


void RRMApplication::getVolumeDimensionsFromController() const
{

    double ox_ = 0, oy_ = 0, oz_ = 0;
    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

    double width_ = 0, height_ = 0, length_ = 0;
    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );

    std::size_t disc_ = 1;
    double step_ = 1.0;
    mainwindow->controller->setupCrossSectionDiscretization( disc_, step_ );


    mainwindow->object_properties->changeVolumeSize( ox_ + width_, oy_ + height_, oz_ + length_ );
    mainwindow->sl_depth_csection->setDiscretization( static_cast< int > ( disc_ ) );
    mainwindow->sl_depth_csection->setRange( oz_,  oz_ + length_ );



}


void RRMApplication::changeVolumeDimension( const AxesDirection& dir_, double value_ )
{

    double ox_ = 0, oy_ = 0, oz_ = 0;
    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

    double width_ = 0, height_ = 0, length_ = 0;
    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


    if( dir_ == AxesDirection::X )
    {
        std::cout << "Inside RRMApplication::changeVolumeDimension, changing X: " << value_ << std::endl << std::flush;

        mainwindow->controller->setVolumeDimensions( value_, height_, length_ );
    }
    else if( dir_ == AxesDirection::Y )
    {
        mainwindow->controller->setVolumeDimensions( width_, value_, length_ );
    }
    else if( dir_ == AxesDirection::Z )
    {
        std::cout << "Inside RRMApplication::changeVolumeDimension, changing Z: " << value_ << std::endl << std::flush;
        std::size_t disc_ = 1;
        double step_ = 1.0;
        mainwindow->controller->setVolumeDimensions( width_, height_, value_ );
        mainwindow->controller->setupCrossSectionDiscretization( disc_, step_ );
        mainwindow->sl_depth_csection->setDiscretization( static_cast< int > ( disc_ ) );
        mainwindow->sl_depth_csection->setRange( oz_,  oz_ + value_ );
    }

}


void RRMApplication::changeVolumeDimensions( const CrossSection::Direction& dir_, double dim1_, double dim2_ )
{
    double ox_ = 0, oy_ = 0, oz_ = 0;
    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

    double width_ = 0, height_ = 0, length_ = 0;
    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


    if( dir_ == CrossSection::Direction::X )
    {
        mainwindow->controller->setVolumeDimensions( width_, dim2_, dim1_ );
    }
    else if( dir_ == CrossSection::Direction::Y )
    {
        mainwindow->controller->setVolumeDimensions( dim1_, height_, dim2_ );
    }
    else if( dir_ == CrossSection::Direction::Z )
    {
        mainwindow->controller->setVolumeDimensions( dim1_, dim2_, length_ );
    }

    getVolumeDimensionsFromController();
}


void RRMApplication::setVolumeName( std::size_t index_, const std::string& name_ )
{
    mainwindow->controller->setVolumeName( name_ );
}


void RRMApplication::setVolumeVisible( std::size_t index_, bool status_ )
{
    if( index_ == 0 )
        mainwindow->controller->setVolumeVisibility( status_ );

}




void RRMApplication::setCurrentCrossSection( double depth_ )
{
    mainwindow->controller->setCurrentCrossSection( depth_ );
}




void RRMApplication::setObjectName( std::size_t index_, const std::string& name_ )
{
    mainwindow->controller->setObjectName( index_, name_ );
}


void RRMApplication::setObjectVisible( std::size_t index_, bool status_ )
{
    mainwindow->controller->setObjectVisibility( index_, status_ );
}


void RRMApplication::setObjectColor( std::size_t index_, int r_, int g_, int b_ )
{
    mainwindow->controller->setObjectColor( index_, r_, g_, b_ );
    updateSketchingCanvas();
}


void RRMApplication::saveObjectInformation( const std::string & text_ )
{
    QList< QTreeWidgetItem* > items = mainwindow->object_tree->selectedItems();
    if( items.empty() == true ) return;

    std::size_t index_ = static_cast< ObjectTreeItem *> (items[ 0 ] )->getIndex();
    mainwindow->controller->saveObjectInformation( index_, text_ );
}


void RRMApplication::getObjectInformation( QTreeWidgetItem* const& item_ ) const
{
    ObjectTreeItem* obj_item_ = static_cast< ObjectTreeItem* >( item_ );

    if( obj_item_->getType() == ObjectTreeItem::Type::VOLUME )
        mainwindow->object_properties->setCurrentIndex( 0 );
    else
        mainwindow->object_properties->setCurrentIndex( 1 );

    std::string text_ = mainwindow->controller->getObjectInformation( obj_item_->getIndex() );
    mainwindow->object_properties->loadObjectInformation( text_ );
}




void RRMApplication::initSketchingApp()
{
    CrossSection* csection_ = mainwindow->controller->getMainCrossSection( DEFAULT_CROSSSECTION_DIRECTION );
    mainwindow->sketch_window->addMainCanvas( csection_ );

    CrossSection* topview_ = mainwindow->controller->getTopViewCrossSection();
    mainwindow->sketch_topview_window->addTopViewCanvas( topview_ );
}



void RRMApplication::updateSketchingCanvas()
{
    mainwindow->sketch_window->updateCanvas();
    mainwindow->sketch_topview_window->updateCanvas();
}


void RRMApplication::acceptSketchingCurve( const PolyCurve& curve_ )
{

    bool status_ = mainwindow->controller->addObjectCurve( curve_ );
    if( status_ == false ) return;

    CrossSection* csection_ = mainwindow->controller->getCurrentCrossSection();
    mainwindow->sketch_topview_window->addCrossSection( csection_ );
    
    updateSketchingCanvas();
    
}


void RRMApplication::acceptSketchingTrajectory( const PolyCurve& curve_ )
{

    bool status_ = mainwindow->controller->addObjectTrajectory( curve_ );
    if( status_ == false ) return;

    updateSketchingCanvas();

}


void RRMApplication::createObjectSurface()
{
    bool status_ = mainwindow->controller->createObjectSurface();
    if( status_ == false ) return;

    mainwindow->controller->addObject();
    mainwindow->object_properties->setEnabledVolumeResize( mainwindow->controller->isVolumeResizable() );

    mainwindow->sketch_window->addObject( mainwindow->controller->getCurrentObject() );
    mainwindow->sketch_topview_window->addTrajectory( mainwindow->controller->getCurrentObject() );

    updateSketchingCanvas();
    defineRandomColor();
    checkUndoRedo();
}




void RRMApplication::setStratigraphicRule( const StratigraphicRules& rules_ )
{
    if( rules_ == StratigraphicRules::REMOVE_ABOVE )
        mainwindow->controller->setRemoveAbove();
    else if( rules_ == StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        mainwindow->controller->setRemoveAboveIntersection();
    else if( rules_ == StratigraphicRules::REMOVE_BELOW )
        mainwindow->controller->setRemoveBelow();
    else if( rules_ == StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        mainwindow->controller->setRemoveBelowIntersection();
}




void RRMApplication::setSketchAbove( bool status_ )
{

    bool enabled_ = mainwindow->controller->enableCreateAbove( status_ );

    if( ( status_ == false ) && ( enabled_ == true ) )
    {
        std::cout << "Stop failed, so keep turned it on" << std::endl << std::flush;
        mainwindow->ac_sketch_above->setChecked( true );
    }
    if( ( status_ == true ) && ( enabled_ == false ) )
    {
        std::cout << "Request failed, so keep turned it off" << std::endl << std::flush;
        mainwindow->ac_sketch_above->setChecked( false );
    }

    if( ( status_ == true ) && ( enabled_ == true ) )
    {
        std::cout << "Request accepted, get the selectable objects" << std::endl << std::flush;
        std::cout << "and mark them as selectable" << std::endl << std::flush;

        setModeSelecting();
//        emit updateObjects();

    }
    if( ( status_ == false ) && ( enabled_ == false ) )
    {
        std::cout << "Stop accepted, get the selectable objects" << std::endl << std::flush;
        std::cout << "and mark them as non-selectable" << std::endl << std::flush;
        setModeSketching();
//        emit updateObjects();
    }
}


void RRMApplication::setSketchBelow( bool status_ )
{

    bool enabled_ = mainwindow->controller->enableCreateBelow( status_ );


    if( ( status_ == false ) && ( enabled_ == true ) )
    {
        std::cout << "Stop failed, so keep turned it on" << std::endl << std::flush;
        mainwindow->ac_sketch_below->setChecked( true );
    }
    if( ( status_ == true ) && ( enabled_ == false ) )
    {
        std::cout << "Request failed, so keep turned it off" << std::endl << std::flush;
        mainwindow->ac_sketch_below->setChecked( false );
    }

    if( ( status_ == true ) && ( enabled_ == true ) )
    {
        std::cout << "Request accepted, get the selectable objects" << std::endl << std::flush;
        std::cout << "and mark them as selectable" << std::endl << std::flush;
        setModeSelecting();

    }
    if( ( status_ == false ) && ( enabled_ == false ) )
    {
        std::cout << "Stop accepted, get the selectable objects" << std::endl << std::flush;
        std::cout << "and mark them as non-selectable" << std::endl << std::flush;
        setModeSketching();
    }
}




void RRMApplication::setObjectAsBoundering( std::size_t index_ )
{
    mainwindow->controller->setObjectAsBoundering( index_ );
    setModeSketching();
}




void RRMApplication::setModeSelecting()
{
    mainwindow->sketch_window->setModeSelecting();
    updateSketchingCanvas();
}


void RRMApplication::setModeSketching()
{
    mainwindow->sketch_window->setModeSketching();
    updateSketchingCanvas();
}



void RRMApplication::save( const std::string& filename_ )
{
    mainwindow->controller->saveFile( filename_ );
}


void RRMApplication::load( const std::string& filename_ )
{
    mainwindow->controller->loadFile( filename_ );
}



void RRMApplication::undo()
{
    mainwindow->controller->undo();
    checkUndoRedo();
}


void RRMApplication::redo()
{
    mainwindow->controller->redo();
    checkUndoRedo();
}


void RRMApplication::checkUndoRedo()
{
    if( mainwindow->controller == nullptr ) return;

    mainwindow->ac_undo->setEnabled(  mainwindow->controller->canUndo() );
    mainwindow->ac_redo->setEnabled(  mainwindow->controller->canRedo() );

    updateSketchingCanvas();
}


void RRMApplication::checkSketchStatus()
{
    if( mainwindow->controller == nullptr ) return;

    mainwindow->ac_sketch_above->setChecked(  mainwindow->controller->isDefineAboveActive() );
    mainwindow->ac_sketch_below->setChecked(  mainwindow->controller->isDefineBelowActive() );

    updateSketchingCanvas();
}



void RRMApplication::setCurrentColor( int r_, int g_, int b_ )
{
    mainwindow->controller->setCurrentColor( r_, g_, b_ );
    mainwindow->sketch_topview_window->setCurrentColor( r_, g_, b_ );
}



void RRMApplication::defineRandomColor()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    int r_ = distr( eng );
    int g_ = distr( eng );
    int b_ = distr( eng );

    mainwindow->sketch_window->setCurrentColor( r_, g_, b_ );
    mainwindow->sketch_topview_window->setCurrentColor( r_, g_, b_ );
    mainwindow->controller->setCurrentColor( r_, g_, b_ );

}


void RRMApplication::clear()
{
    mainwindow->sketch_window->clear();
    mainwindow->sketch_topview_window->clear();
    mainwindow->controller->clear();


    mainwindow->controller->init();
    setRRMDefaultValuesOnInterface();
    mainwindow->controller->addMainCrossSection( DEFAULT_CROSSSECTION_DIRECTION, DEFAULT_CROSSSECTION_DEPTHZ );
    mainwindow->controller->addTopViewCrossSection( DEFAULT_CROSSSECTION_DEPTHY );

    initSketchingApp();
}
