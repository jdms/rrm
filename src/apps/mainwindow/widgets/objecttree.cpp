#include "objecttree.h"

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
    if( vol_ != nullptr )
        vol_->addChild( obj_ );
}
