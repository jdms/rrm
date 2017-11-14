#include <iostream>
#include <QCheckBox>

#include "objecttree.h"
#include "./mainwindow/widgets/color_picker.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( COLUMNS_NUMBER );

    connect( this, &ObjectTree::itemChanged, this, &ObjectTree::filterAction );

}



void ObjectTree::filterAction( QTreeWidgetItem* item_, std::size_t column_ )
{
    ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( item_ );

    if( obj_->getType() == ObjectTreeItem::Type::VOLUME )
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

    else if( obj_->getType() == ObjectTreeItem::Type::STRATIGRAPHY )
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

    else if( obj_->getType() == ObjectTreeItem::Type::CROSS_SECTION )
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

    else if( obj_->getType() == ObjectTreeItem::Type::REGION )
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
    vol_->setType( ObjectTreeItem::Type::VOLUME );
    vol_->setIndex( 0 );
    vol_->setCheckState( COLUMN_STATUS, Qt::Checked );
    addTopLevelItem( vol_ );

}

void ObjectTree::addOutputVolume()
{
    ObjectTreeItem* vol_ = new ObjectTreeItem();
    vol_->setText( COLUMN_NAME, "OUTPUT VOLUME" );
    vol_->setType( ObjectTreeItem::Type::VOLUME );
    vol_->setIndex( 1 );
    vol_->setCheckState( COLUMN_STATUS, Qt::Checked );
    addTopLevelItem( vol_ );

}


void ObjectTree::addObject( std::size_t index_, const ObjectTreeItem::Type& type_,
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

    vol_->addChild( obj_ );

    ColorPicker* colorpicker_ = new ColorPicker( this );
    colorpicker_->setMaximumWidth( 30 );
    colorpicker_->setColor( QColor( red_, green_, blue_ ) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit setObjectColor( index_, color_ ); } );

    setItemWidget( obj_, COLUMN_COLOR, colorpicker_ );
    items.addElement( index_, obj_ );

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
    region_->setType( ObjectTreeItem::Type::REGION );
    region_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    region_->setCheckState( COLUMN_STATUS, Qt::Checked );


    ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 1 );
    if( vol1_ == nullptr ) return;

    vol1_->addChild( region_ );

    ColorPicker* colorpicker_ = new ColorPicker( this );
    colorpicker_->setMaximumWidth( 30 );
    colorpicker_->setColor( QColor( red_, green_, blue_ ) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit setObjectColor( index_, color_ ); } );

    setItemWidget( region_, COLUMN_COLOR, colorpicker_ );
    regions.addElement( index_, region_ );
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

    ObjectTreeItem* region_ = regions.getElement( index_ );
    region_->setHidden( !status_ );
}


void ObjectTree::clear()
{

    std::vector < std::size_t > diff;

    for ( int i = 0; i < topLevelItemCount(); ++i )
    {

        int nchildren = topLevelItem(i)->childCount();
        for( int j = 0; j < nchildren; ++j )
        {
            ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem(i)->child( j ) );

//            obj_->clear();

            ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
            removeItemWidget( obj_, COLUMN_COLOR );
            delete colorpicker_;

            if( items.findElement( obj_->getIndex() ) == true )
            {
                diff.push_back( obj_->getIndex() );
                continue;
            }

            delete obj_;
            obj_ = nullptr;
        }
    }

    for( auto d: diff )
    {
        ObjectTreeItem* obj_ = items.getElement( d );
        delete obj_;
        obj_ = nullptr;
    }

    QTreeWidget::clear();
    items.clear();



}
