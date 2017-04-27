#include "ObjectTree.h"

ObjectTree::ObjectTree( QWidget *parent )
{

}


void ObjectTree::addInputVolume( ObjectTreeItem* const& vol_ )
{
    input_volume = vol_;
    input_volume->setText( 0, "INPUT_VOLUME" );
    this->addTopLevelItem( input_volume );
}


void ObjectTree::addObject( ObjectTreeItem* const& obj_ )
{
    std::size_t id_ = obj_->getId();

    auto search = objects.find( id_ );
    if( search != objects.end() ) return;

    obj_->setText( 0, QString( "Object %1" ).arg( id_ ) );
    objects[ id_ ] = obj_;

    input_volume->addChild( obj_ );

}

void ObjectTree::addRegion( ObjectTreeItem* const& reg_ )
{
    std::size_t id_ = reg_->getId();

    auto search = regions.find( id_ );
    if( search != regions.end() ) return;

    reg_->setText( 0, QString( "Region %1" ).arg( id_) );
    regions[ id_ ] = reg_;

    input_volume->addChild( reg_);
}
