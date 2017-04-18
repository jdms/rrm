#include "TreeItem.h"

TreeItem::TreeItem( const QList< QVariant >& data_, TreeItem *parent_ )
{
    parent = parent_;
    itemData = data_;
}

TreeItem::~TreeItem()
{
    qDeleteAll( childItems );
}

int TreeItem::row() const
{
    if( parent )
        return parent->childItems.indexOf( const_cast< TreeItem* >(this) );
    return 0;
}


