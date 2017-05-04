#include <iostream>
#include <QtWidgets/QCheckBox>


#include "ObjectTree.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( 2 );
    setColumnWidth( 0, 60 );
}


void ObjectTree::addInputVolume(/* ObjectTreeItem* const& vol_ */)
{

    input_volume = new ObjectTreeItem( ObjectTreeItem::TreeItemType::VOLUME, 0 );
    input_volume->setText( 1, "INPUT_VOLUME" );

    QCheckBox* chk_input_volume = new QCheckBox( this );
    connect( chk_input_volume, &QCheckBox::toggled, [=]( bool status_ )
                                                    { emit setInputVolumeVisible( status_ ); } );

    this->addTopLevelItem( input_volume );
    setItemWidget( input_volume, 0,  chk_input_volume );

}


void ObjectTree::addObject( ObjectTreeItem* const& obj_ )
{
    std::size_t id_ = obj_->getId();

    auto search = objects.find( id_ );
    if( search != objects.end() ) return;


    obj_->setText( 1, QString( "Surface %1" ).arg( id_ ) );
    objects[ id_ ] = obj_;

    QCheckBox* chk_object = new QCheckBox( this );
    connect( chk_object, &QCheckBox::toggled, [=]( bool status_ )
                                                    { emit setObjectVisible( id_, status_ ); } );


    input_volume->addChild( obj_ );
    setItemWidget( obj_, 0, chk_object );

}

void ObjectTree::addRegion( ObjectTreeItem* const& reg_ )
{
    std::size_t id_ = reg_->getId();

    auto search = regions.find( id_ );
    if( search != regions.end() ) return;

    reg_->setText( 1, QString( "Region %1" ).arg( id_) );
    regions[ id_ ] = reg_;

    input_volume->addChild( reg_);
    setItemWidget( reg_, 0, new QCheckBox( this ) );
}
