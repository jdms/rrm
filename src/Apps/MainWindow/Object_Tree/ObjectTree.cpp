#include <iostream>
#include <QtWidgets/QCheckBox>

#include "../Widgets/ColorPicker.h"
#include "ObjectTree.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( 3 );
    setColumnWidth( 0, 60 );
    setColumnWidth( 1, 150 );
    setColumnWidth( 2, 40 );
}

void ObjectTree::filterAction( QTreeWidgetItem* item, std::size_t column )
{
    ObjectTreeItem* const& obj = static_cast< ObjectTreeItem* >( item );
    QCheckBox* const& chk = static_cast< QCheckBox* >( itemWidget( item, 0 ) );
    ColorPicker* const& cp = static_cast< ColorPicker* >( itemWidget( item, 2 ) );


    std::size_t id = obj->getId();

    if( ( obj->getType() == ObjectTreeItem::TreeItemType::OBJECT ) && ( column == 0 ) )
        emit setObjectVisible( id, chk->isChecked() );

    if( ( obj->getType() == ObjectTreeItem::TreeItemType::OBJECT ) && ( column == 1 ) )
        emit setObjectName( id, item->text( 1 ) );

    else if( ( obj->getType() == ObjectTreeItem::TreeItemType::OBJECT ) && ( column == 2 ) )
        emit setObjectColor( id, cp->currentColor() );


}


ObjectTreeItem* ObjectTree::addVolumeChilds( const ObjectTreeItem::TreeItemType& type, std::size_t id )
{

    ObjectTreeItem* item = new ObjectTreeItem( type, id );

    QCheckBox* checkbox = new QCheckBox( this );
    checkbox->setChecked( true );

    ColorPicker* colorpicker = new ColorPicker( this );
    colorpicker->setMaximumSize( 35, 30 );

    input_volume->addChild( item );
    setItemWidget( item, 0, checkbox );
    setItemWidget( item, 2, colorpicker );

    connect( checkbox, &QCheckBox::toggled,  [=](){ filterAction( item, 0 ); } );
    connect( colorpicker, &ColorPicker::colorSelected, [=](){ filterAction( item, 2 ); } );
    connect( this, &ObjectTree::itemChanged, this, &ObjectTree::filterAction );

    return item;
}


void ObjectTree::addInputVolume()
{

    input_volume = new ObjectTreeItem( ObjectTreeItem::TreeItemType::VOLUME, 0 );
    input_volume->setText( 1, "INPUT_VOLUME" );

    QCheckBox* chk_input_volume = new QCheckBox( this );
    chk_input_volume->setChecked( true );
    connect( chk_input_volume, &QCheckBox::toggled, [=]( bool status_ )
                                                    { emit setInputVolumeVisible( status_ ); } );

    this->addTopLevelItem( input_volume );
    setItemWidget( input_volume, 0,  chk_input_volume );

}


void ObjectTree::addObject( const std::size_t& id , int r, int g, int b )
{

    auto search = objects.find( id );
    if( search != objects.end() ) return;

    ObjectTreeItem* const& item = addVolumeChilds( ObjectTreeItem::TreeItemType::OBJECT, id );
    item->setText( 1, QString( "Surface %1" ).arg( id ) );

    ColorPicker* const& cp = static_cast< ColorPicker* >( itemWidget( item, 2 ) );
    cp->setColor( QColor( r, g, b ) );

    objects[ id ] = item;

}


void ObjectTree::addRegion( const std::size_t& id )
{

    auto search = regions.find( id );
    if( search != regions.end() ) return;

    ObjectTreeItem* const& item = addVolumeChilds( ObjectTreeItem::TreeItemType::REGION, id );
    item->setText( 1, QString( "Region %1" ).arg( id ) );

    QCheckBox* chk = static_cast<QCheckBox*>( itemWidget( item, 0 ) );
//    connect( chk, &QCheckBox::toggled, [=]( bool status ){ emit setObjectVisible( id, status ); } );

    regions[ id ] = item;

  }


void ObjectTree::clear()
{



    for( auto& it: objects )
    {
        delete objects[ it.first ];
    }
    objects.clear();

    QTreeWidget::clear();
}
