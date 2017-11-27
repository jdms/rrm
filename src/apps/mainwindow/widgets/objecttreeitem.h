#ifndef OBJECTTREEITEM_H
#define OBJECTTREEITEM_H


#include <QTreeWidgetItem>


class ObjectTreeItem: public QTreeWidgetItem
{
    public:

        enum class Type { VOLUME, CROSS_SECTION, STRATIGRAPHY, REGION, WELL };


        ObjectTreeItem();

        void setType( const ObjectTreeItem::Type& type_ );
        ObjectTreeItem::Type getType() const;


        void setIndex( const std::size_t index_ );
        std::size_t getIndex() const;

        void setTreeIndex( const std::size_t index_ );
        std::size_t getTreeIndex() const;



    protected:


        std::size_t index;
        std::size_t tree_index;
        Type type;


};

#endif // OBJECTTREEITEM_H
