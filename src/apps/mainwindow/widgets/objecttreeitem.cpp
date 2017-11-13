#include "objecttreeitem.h"

ObjectTreeItem::ObjectTreeItem()
{
    setFlags( flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable );
}

void ObjectTreeItem::setType( const ObjectTreeItem::Type& type_ )
{
    type = type_;
}

ObjectTreeItem::Type ObjectTreeItem::getType() const
{
    return type;
}


void ObjectTreeItem::setIndex( const std::size_t index_ )
{
    index = index_;
}

std::size_t ObjectTreeItem::getIndex() const
{
    return index;
}
