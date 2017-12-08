#include "objecttreeitem.h"

ObjectTreeItem::ObjectTreeItem()
{
    setFlags( flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable );
}

void ObjectTreeItem::setType( const Settings::Objects::ObjectType& type_ )
{
    type = type_;
}

Settings::Objects::ObjectType ObjectTreeItem::getType() const
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


void ObjectTreeItem::setTreeIndex( const std::size_t index_ )
{
    tree_index = index_;
}

std::size_t ObjectTreeItem::getTreeIndex() const
{
    return tree_index;
}
