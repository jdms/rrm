#include <iostream>
#include <QtWidgets/QCheckBox>


#include "ObjectTree.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( 2 );
    setColumnWidth( 0, 60 );
}


void ObjectTree::addInputVolume()
{

    input_volume = new ObjectTreeItem( ObjectTreeItem::TreeItemType::VOLUME, 0 );
    input_volume->setText( 1, "INPUT_VOLUME" );

    QCheckBox* chk_input_volume = new QCheckBox( this );
    connect( chk_input_volume, &QCheckBox::toggled, [=]( bool status_ )
                                                    { emit setInputVolumeVisible( status_ ); } );

    this->addTopLevelItem( input_volume );
    setItemWidget( input_volume, 0,  chk_input_volume );

}


void ObjectTree::addObject( const std::size_t& id_ )
{

    auto search = objects.find( id_ );
    if( search != objects.end() ) return;

    ObjectTreeItem* item_ = new ObjectTreeItem( ObjectTreeItem::TreeItemType::OBJECT, id_ );
    item_->setText( 1, QString( "Surface %1" ).arg( id_ ) );
    objects[ id_ ] = item_;

    QCheckBox* chk_object = new QCheckBox( this );
    connect( chk_object, &QCheckBox::toggled, [=]( bool status_ )
                                                    { emit setObjectVisible( id_, status_ ); } );


    input_volume->addChild( item_ );
    setItemWidget( item_, 0, chk_object );

}

void ObjectTree::addRegion( const std::size_t& id_ )
{

    auto search = regions.find( id_ );
    if( search != regions.end() ) return;

    ObjectTreeItem* item_ = new ObjectTreeItem( ObjectTreeItem::TreeItemType::REGION, id_ );
    item_->setText( 1, QString( "Region %1" ).arg( id_) );
    regions[ id_ ] = item_;

    input_volume->addChild( item_ );
    setItemWidget( item_, 0, new QCheckBox( this ) );
}
