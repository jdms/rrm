#ifndef OBJECTTREEITEM_H
#define OBJECTTREEITEM_H

#include <QTreeWidgetItem>


class ObjectTreeItem: public QTreeWidgetItem
{
    public:

        enum class TreeItemType{ VOLUME, OBJECT, REGION };

        inline ObjectTreeItem( const TreeItemType& type_, std::size_t id_ )
        {
            type = type_;
            id = id_;

            setFlags( flags() | Qt::ItemIsEditable );
        }

        inline TreeItemType getType() const{ return type; }
        inline void setId( std::size_t id_ ){ id = id_; }
        inline std::size_t getId() const{ return id; }

    protected:

        TreeItemType type;
        std::size_t id;




};

#endif // OBJECTTREEITEM_H
