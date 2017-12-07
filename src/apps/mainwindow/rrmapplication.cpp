#include "rrmapplication.h"
#include "mainwindow.h"



std::size_t Object::number_of_objects = 0;
std::size_t CrossSection::number_of_csections = 0;



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

    mainwindow->controller->addMainCrossSection( Settings::CrossSection::DEFAULT_CSECTION_DIRECTION, Settings::CrossSection::INITIAL_CSECTIONZ_POSITION );
    mainwindow->controller->addTopViewCrossSection( Settings::CrossSection::INITIAL_CSECTIONY_POSITION  );


}


void RRMApplication::setSiderBarVisibility( bool status_ )
{
    mainwindow->dw_object_properties->setVisible( status_ );
    mainwindow->dw_object_tree->setVisible( status_ );
}


void RRMApplication::setDefaultRule( Settings::Stratigraphy::StratigraphicRules rule_ )
{
    if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
        mainwindow->ac_remove_above->setChecked( Variables::ON );
    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        mainwindow->ac_remove_above_int->setChecked( Variables::ON );
    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
        mainwindow->ac_remove_below->setChecked( Variables::ON );
    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        mainwindow->ac_remove_below_int->setChecked( Variables::ON );
}


void RRMApplication::setDefaultSketchingRegion( Settings::Objects::BounderingRegion sketching_region_ )
{
    if( sketching_region_ == Settings::Objects::BounderingRegion::ABOVE )
        mainwindow->ac_sketch_above->setChecked( Variables::ON );
    else if( sketching_region_ == Settings::Objects::BounderingRegion::BELOW )
        mainwindow->ac_sketch_below->setChecked( Variables::ON );
    else
    {
        mainwindow->ac_sketch_above->setChecked( Variables::OFF );
        mainwindow->ac_sketch_below->setChecked( Variables::OFF );
    }

}


void RRMApplication::setDefaultSiderBarValues()
{
    mainwindow->object_properties->setEnabledVolumeResize( mainwindow->controller->isVolumeResizable() );
}


void RRMApplication::setRRMDefaultValuesOnInterface()
{

    setSiderBarVisibility( Settings::Application::DEFAULT_SIDEBAR_VISIBILITY );
    setDefaultRule( Settings::Stratigraphy::DEFAULT_STRAT_RULES );
    setDefaultSketchingRegion( Settings::Objects::BounderingRegion::NONE );
    setDefaultSiderBarValues();

    setVolumeOriginToController( Settings::Volume::VOLUME_ORIGINX, Settings::Volume::VOLUME_ORIGINY, Settings::Volume::VOLUME_ORIGINZ );
    setVolumeDimensionsToController( Settings::Volume::VOLUME_WIDTH, Settings::Volume::VOLUME_HEIGHT, Settings::Volume::VOLUME_LENGTH );

}



void RRMApplication::setVolumeOriginToController( double ox_, double oy_, double oz_ )
{
    mainwindow->controller->setVolumeOrigin( ox_, oy_, oz_ );
    getVolumeDimensionsFromController();
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


void RRMApplication::changeVolumeDimension( const Settings::CrossSection::CrossSectionDirections& dir_, double value_ )
{

    double ox_ = 0, oy_ = 0, oz_ = 0;
    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

    double width_ = 0, height_ = 0, length_ = 0;
    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        std::cout << "Inside RRMApplication::changeVolumeDimension, changing X: " << value_ << std::endl << std::flush;

        mainwindow->controller->setVolumeDimensions( value_, height_, length_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        mainwindow->controller->setVolumeDimensions( width_, value_, length_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
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


void RRMApplication::changeVolumeDimensions( const Settings::CrossSection::CrossSectionDirections& dir_, double dim1_, double dim2_ )
{
    double ox_ = 0, oy_ = 0, oz_ = 0;
    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

    double width_ = 0, height_ = 0, length_ = 0;
    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        mainwindow->controller->setVolumeDimensions( width_, dim2_, dim1_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        mainwindow->controller->setVolumeDimensions( dim1_, height_, dim2_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
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

    if( obj_item_->getType() == Settings::Objects::ObjectType::VOLUME )
        mainwindow->object_properties->setCurrentIndex( 0 );
    else if( obj_item_->getType() == Settings::Objects::ObjectType::STRATIGRAPHY )
    {
        mainwindow->object_properties->setCurrentIndex( 1 );
        std::string text_ = mainwindow->controller->getObjectInformation( obj_item_->getIndex() );
        std::string name_ = mainwindow->controller->getObjectName( obj_item_->getIndex() );
        mainwindow->object_properties->loadObjectInformation( name_, text_ );
    }

}




void RRMApplication::setRegionName( std::size_t index_, const std::string& name_ )
{
    mainwindow->controller->setRegionName( index_, name_ );
}


void RRMApplication::setRegionVisible( std::size_t index_, bool status_ )
{
    mainwindow->controller->setRegionVisibility( index_, status_ );
}


void RRMApplication::setRegionColor( std::size_t index_, int r_, int g_, int b_ )
{
    mainwindow->controller->setRegionColor( index_, r_, g_, b_ );
//    updateSketchingCanvas();
}



void RRMApplication::initSketchingApp()
{
//    CrossSection* csection_ = mainwindow->controller->getMainCrossSection( Settings::CrossSection::DEFAULT_CSECTION_DIRECTION );
    mainwindow->sketch_window->addMainCanvas( mainwindow->controller->getMainCrossSection( Settings::CrossSection::DEFAULT_CSECTION_DIRECTION )/*csection_*/ );
    mainwindow->dw_sketchwindow->setVisible( Settings::Application::DEFAULT_CSECTION_VISIBILITY );

//    CrossSection* topview_ = mainwindow->controller->getTopViewCrossSection();
    mainwindow->sketch_topview_window->addTopViewCanvas( mainwindow->controller->getTopViewCrossSection()/*topview_*/ );
    mainwindow->dw_topview_window->setVisible( Settings::Application::DEFAULT_TOPVIEW_VISIBILITY );
}



void RRMApplication::updateSketchingCanvas()
{
    mainwindow->sketch_window->updateCanvas();
    mainwindow->sketch_topview_window->updateCanvas();
}


void RRMApplication::acceptSketchingCurve( const PolyCurve& curve_, double depth_ )
{

    bool status_ = mainwindow->controller->addObjectCurve( curve_, depth_ );
    if( status_ == false ) return;

    CrossSection* csection_ = mainwindow->controller->getCrossSection( depth_ );
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




void RRMApplication::setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ )
{
    if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
        mainwindow->controller->setRemoveAbove();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        mainwindow->controller->setRemoveAboveIntersection();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
        mainwindow->controller->setRemoveBelow();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        mainwindow->controller->setRemoveBelowIntersection();
}




void RRMApplication::setSketchAbove( bool status_ )
{

    bool below_done = mainwindow->controller->isDefineBelowObjectSelected();
    if( below_done == false )
    {
        mainwindow->ac_sketch_above->setChecked( !status_ );
        return;
    }

    bool enabled_ = mainwindow->controller->enableCreateAbove( status_ );


    if( status_ == false )
    {
        if( enabled_ == false )
        {
            mainwindow->ac_sketch_above->setChecked( false );
            setModeSketching();
        }

        updateSketchingCanvas();

    }

    else if(  status_ == true )
    {
        if( enabled_ == true )
            setModeSelecting();
        else
            setModeSketching();
    }

}


void RRMApplication::setSketchBelow( bool status_ )
{

    bool above_done = mainwindow->controller->isDefineAboveObjectSelected();
    if( above_done == false )
    {
        mainwindow->ac_sketch_below->setChecked( !status_ );
        return;
    }

    bool enabled_ = mainwindow->controller->enableCreateBelow( status_ );

    if( status_ == false )
    {

        if( enabled_ == false )
        {
            mainwindow->ac_sketch_below->setChecked( false );
            setModeSketching();
        }

         updateSketchingCanvas();

    }

    else if(  status_ == true )
    {
        if( enabled_ == true )
            setModeSelecting();
        else
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
    clear();
    mainwindow->controller->loadFile( filename_ );
    checkUndoRedo();
    checkSketchStatus();
    initSketchingApp();
}





void RRMApplication::undo()
{
    mainwindow->controller->undo();
    checkUndoRedo();
    checkSketchStatus();
}


void RRMApplication::redo()
{
    mainwindow->controller->redo();
    checkUndoRedo();
    checkSketchStatus();
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



void RRMApplication::clearInterface()
{
    mainwindow->sl_depth_csection->clear();
    mainwindow->object_properties->clear();
}


void RRMApplication::clear()
{
    clearInterface();

    mainwindow->sketch_window->clear();
    mainwindow->sketch_topview_window->clear();
    mainwindow->controller->clear();


    mainwindow->controller->init();
    setRRMDefaultValuesOnInterface();
    mainwindow->controller->addMainCrossSection( Settings::CrossSection::DEFAULT_CSECTION_DIRECTION, Settings::CrossSection::INITIAL_CSECTIONZ_POSITION );
    mainwindow->controller->addTopViewCrossSection( Settings::CrossSection::INITIAL_CSECTIONY_POSITION );

}


void RRMApplication::restart()
{
    clear();
    initSketchingApp();
}



void RRMApplication::addCrossSectionCanvas( double depth_ )
{
    bool status_ = mainwindow->controller->addFixedCrossSection( depth_ );
    if( status_ == false ) return;
    mainwindow->sketch_window->addFixedCrossSectionCanvas( mainwindow->controller->getCrossSection( depth_ ) );
    updateSketchingCanvas();
}


void RRMApplication::removeCrossSectionCanvas( double depth_ )
{
    bool status_ = mainwindow->sketch_window->removeFixedCrossSectionCanvas( depth_ );
    if( status_ == false ) return;

    status_ = mainwindow->controller->removeFixedCrossSection( depth_ );

}


void RRMApplication::setImageToCrossSection( double depth_, std::string file_, double ox_, double oy_, double x_, double y_ )
{
    mainwindow->controller->setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ );
}


void RRMApplication::getHeightMapTopView()
{
    double w_, h_, l_;
    mainwindow->controller->getVolumeDimensions( w_, h_, l_ );

    std::string image_ = mainwindow->canvas3d->sendImage( w_, l_ );
    mainwindow->sketch_topview_window->setTopViewImage( image_ );
}


void RRMApplication::removeCurveFromObject(  double depth_, std::size_t index_ )
{
     mainwindow->controller->removeObjectCurve( index_, depth_ );
     updateSketchingCanvas();
}


void RRMApplication::removeImageFromCrossSection( double depth_ )
{
    mainwindow->controller->clearImageCrossSection( depth_ );
    updateSketchingCanvas();
}

