#include <iostream>
#include <QCheckBox>

#include "objecttree.h"
#include "./core/widgets/color_picker.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( COLUMNS_NUMBER );
    connect( this, &ObjectTree::itemChanged, this, &ObjectTree::filterAction );

}



void ObjectTree::filterAction( QTreeWidgetItem* item_, std::size_t column_ )
{
    ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( item_ );

    if( obj_->getType() == Settings::Objects::ObjectType::VOLUME )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setVolumeVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setVolumeName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

    else if( obj_->getType() == Settings::Objects::ObjectType::STRATIGRAPHY )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setObjectVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setObjectName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }

    }

    else if( obj_->getType() == Settings::Objects::ObjectType::CROSS_SECTION )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setCrossSectionVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setCrossSectionName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

    else if( obj_->getType() == Settings::Objects::ObjectType::REGION )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setRegionVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setRegionName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }
}




void ObjectTree::addInputVolume()
{
    ObjectTreeItem* vol_ = new ObjectTreeItem();
    vol_->setText( COLUMN_NAME, "VOLUME" );
    vol_->setType( Settings::Objects::ObjectType::VOLUME );
    vol_->setIndex( 0 );
    vol_->setCheckState( COLUMN_STATUS, Qt::Checked );
    addTopLevelItem( vol_ );

}


void ObjectTree::hideInputVolume()
{
    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
    vol_->setHidden( true );
    setVolumeVisible( 0, false );

    int nchildren = vol_->childCount();
    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( vol_->child( j ) );
        obj_->setHidden( true );
        setObjectVisibility( obj_->getIndex(), false );

    }

    update();

}


void ObjectTree::hideOutputVolume()
{
//    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
//    vol_->setHidden( true );
//    setVolumeVisible( 1, false );

//    int nchildren = vol_->childCount();
//    for( int j = 0; j < nchildren; ++j )
//    {

//        ObjectTreeItem* obj_ = (ObjectTreeItem* )( vol_->child( j ) );
////        obj_->setHidden( true );
////        setRegionVisibility( obj_->getIndex(), false );

//        std::cout << " Region to be off: " << obj_->getIndex() << std::endl << std::flush;
//    }

//    update();

}


void ObjectTree::addOutputVolume()
{
    ObjectTreeItem* vol_ = new ObjectTreeItem();
    vol_->setText( COLUMN_NAME, "OUTPUT VOLUME" );
    vol_->setType( Settings::Objects::ObjectType::VOLUME );
    vol_->setIndex( 1 );
    vol_->setCheckState( COLUMN_STATUS, Qt::Checked );
    addTopLevelItem( vol_ );
}


void ObjectTree::removeOutputVolume()
{
    if( topLevelItemCount() < 2 )
        return;

    int nchildren = topLevelItem( 1 )->childCount();
    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 1 )->child( j ) );

        ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
        removeItemWidget( obj_, COLUMN_COLOR );
        delete colorpicker_;

        delete obj_;
        obj_ = nullptr;
    }

    ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
    if( vol1_ != nullptr )
    {
        delete vol1_;
        vol1_ = nullptr;
   }

    regions.clear();
}


void ObjectTree::addObject( std::size_t index_, const Settings::Objects::ObjectType& type_,
                            const std::string& name_,  const int& red_,
                            const int& green_,  const int& blue_ )
{
    ObjectTreeItem* obj_ = new ObjectTreeItem();
    obj_->setIndex( index_ );
    obj_->setType( type_ );
    obj_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    obj_->setCheckState( COLUMN_STATUS, Qt::Checked );


    ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
    if( vol_ == nullptr ) return;



    ColorPicker* colorpicker_ = new ColorPicker( this );
    colorpicker_->setColor( QColor( red_, green_, blue_ ) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ )
                                                        { emit setObjectColor( index_, color_ ); } );



    vol_->addChild( obj_ );

    setItemWidget( obj_, COLUMN_COLOR, colorpicker_ );
    items.addElement( index_, obj_ );


    connect( this, &ObjectTree::setVolumeVisible, [=]( std::size_t indexv_, bool status_ )
    {   if( indexv_ != 0 ) return;

        ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 0 );
        if( vol1_ == nullptr ) return;

        ObjectTreeItem* obj1_ = items.getElement( index_ );
        if( obj1_ == nullptr ) return;

        obj1_->setCheckState( COLUMN_STATUS, vol1_->checkState( COLUMN_STATUS) );
    } );
}


void ObjectTree::removeObject( std::size_t index_ )
{
    if( items.findElement( index_ ) == false ) return;
    ObjectTreeItem* obj_ = items.getElement( index_ );
    if( obj_ == nullptr ) return;


    ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
    removeItemWidget( obj_, COLUMN_COLOR );
    delete colorpicker_;


    ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
    if( vol_ == nullptr ) return;
    vol_->removeChild( obj_ );

    delete obj_;
    obj_ = nullptr;

}


void ObjectTree::updateObjectColor( std::size_t index_, int red_, int green_, int blue_)
{
    if( items.findElement( index_ ) == false ) return;

    ObjectTreeItem* obj_ = items.getElement( index_ );
    ColorPicker* cp_color_ = static_cast< ColorPicker* > ( itemWidget( obj_, COLUMN_COLOR ) );
    cp_color_->setColor( QColor( red_, green_, blue_ ) );
}


void ObjectTree::setObjectVisibility( std::size_t index_, bool status_ )
{
    if( items.findElement( index_ ) == false ) return;

    ObjectTreeItem* obj_ = items.getElement( index_ );
    obj_->setHidden( !status_ );
}



void ObjectTree::addRegion( std::size_t index_, const std::string& name_,  const int& red_,
                const int& green_,  const int& blue_ )
{
    ObjectTreeItem* region_ = new ObjectTreeItem();
    region_->setIndex( index_ );
    region_->setType( Settings::Objects::ObjectType::REGION );
    region_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    region_->setCheckState( COLUMN_STATUS, Qt::Checked );


    ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 1 );
    if( vol1_ == nullptr ) return;

    vol1_->addChild( region_ );

    ColorPicker* colorpicker_ = new ColorPicker( this );
    colorpicker_->setColor( QColor( red_, green_, blue_ ) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit setRegionColor( index_, color_ ); } );

    setItemWidget( region_, COLUMN_COLOR, colorpicker_ );
    setColumnWidth( COLUMN_NAME, COLUMN_NAME_WIDTH );
    regions.addElement( index_, region_ );

    connect( this, &ObjectTree::setVolumeVisible, [=]( std::size_t indexv_, bool status_ )
    {   if( indexv_ != 1 ) return;

        ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 1 );
        if( vol1_ == nullptr ) return;

        ObjectTreeItem* region1_ = regions.getElement( index_ );
        if( region1_ == nullptr ) return;

        region1_->setCheckState( COLUMN_STATUS, vol1_->checkState( COLUMN_STATUS) );
    } );


//    connect( this, &ObjectTree::setVolumeVisible, [=]( std::size_t indexv_, bool status_ )
//    {   if( indexv_ != 1 ) return;
//        region_->setCheckState( COLUMN_STATUS, vol1_->checkState( COLUMN_STATUS) );
//    } );
}


void ObjectTree::updateRegionColor( std::size_t index_, int red_, int green_, int blue_)
{

    if( regions.findElement( index_ ) == false ) return;

    ObjectTreeItem* region_ = regions.getElement( index_ );
    ColorPicker* cp_color_ = static_cast< ColorPicker* > ( itemWidget( region_, COLUMN_COLOR ) );
    cp_color_->setColor( QColor( red_, green_, blue_ ) );
}


void ObjectTree::setRegionVisibility( std::size_t index_, bool status_ )
{
    if( regions.findElement( index_ ) == false ) return;

    std::cout << "Found region " << index_ <<std::endl << std::flush;

    ObjectTreeItem* region_ = regions.getElement( index_ );
    region_->setHidden( !status_ );
}


void ObjectTree::clear()
{

    std::vector< std::size_t > diff_objs;
    std::vector< std::size_t > diff_regs;


    if( topLevelItemCount() != 0 )
    {

        int nchildren = topLevelItem( 0 )->childCount();
        for( int j = 0; j < nchildren; ++j )
        {
            ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 0 )->child( j ) );

            ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
            removeItemWidget( obj_, COLUMN_COLOR );
            delete colorpicker_;

            if( items.findElement( obj_->getIndex() ) == true )
            {
                diff_objs.push_back( obj_->getIndex() );
                continue;
            }

            delete obj_;
            obj_ = nullptr;
        }

        for( auto d: diff_objs )
        {
            ObjectTreeItem* obj_ = items.getElement( d );

            delete obj_;
            obj_ = nullptr;
        }



        if( topLevelItemCount() < 2 )
        {
            ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
            if( vol0_ != nullptr )
            {
                delete vol0_;
                vol0_ = nullptr;
            }
            return;
        }

        nchildren = topLevelItem( 1 )->childCount();
        for( int j = 0; j < nchildren; ++j )
        {
            ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 1 )->child( j ) );

            ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
            removeItemWidget( obj_, COLUMN_COLOR );
            delete colorpicker_;

            if( regions.findElement( obj_->getIndex() ) == true )
            {
                diff_regs.push_back( obj_->getIndex() );
                continue;
            }

            delete obj_;
            obj_ = nullptr;
        }

        for( auto d: diff_regs )
        {
            ObjectTreeItem* obj_ = regions.getElement( d );

            delete obj_;
            obj_ = nullptr;
        }


        ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
        if( vol0_ != nullptr )
        {
            delete vol0_;
            vol0_ = nullptr;
        }

         ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
         if( vol1_ != nullptr )
         {
             delete vol1_;
             vol1_ = nullptr;
        }

    }


    QTreeWidget::clear();
    items.clear();
    regions.clear();



}
