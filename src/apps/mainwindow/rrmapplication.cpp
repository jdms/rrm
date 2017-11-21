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


void RRMApplication::changeDimensionVolume( const AxesDirection& dir_, double value_ )
{
    double ox_ = 0, oy_ = 0, oz_ = 0;
    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

    double width_ = 0, height_ = 0, length_ = 0;
    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


    if( dir_ == AxesDirection::X )
    {
        mainwindow->controller->setVolumeDimensions( value_, height_, length_ );
    }
    else if( dir_ == AxesDirection::Y )
    {
        mainwindow->controller->setVolumeDimensions( width_, value_, length_ );
    }
    else if( dir_ == AxesDirection::Z )
    {
        std::size_t disc_ = 1;
        double step_ = 1.0;
        mainwindow->controller->setVolumeDimensions( width_, height_, value_ );
        mainwindow->controller->setupCrossSectionDiscretization( disc_, step_ );
        mainwindow->sl_depth_csection->setDiscretization( static_cast< int > ( disc_ ) );
        mainwindow->sl_depth_csection->setRange( oz_,  oz_ + value_ );
    }

}


void RRMApplication::setVolumeName( std::size_t index_, const std::string& name_ )
{
    mainwindow->controller->setVolumeName( name_ );
}


void RRMApplication::setVolumeVisible( std::size_t index_, bool status_ )
{
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
}
