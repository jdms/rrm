#include "objecttreeitem.h"

ObjectTreeItem::ObjectTreeItem()
{
    setFlags( flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable );
}
