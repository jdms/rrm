#ifndef OBJECTTREEITEM_H
#define OBJECTTREEITEM_H


#include <QTreeWidgetItem>

#include "./core/definitions/constants.hpp"


class ObjectTreeItem: public QTreeWidgetItem
{
    public:


        ObjectTreeItem();

        void setType( const Settings::Objects::ObjectType& type_ );
        Settings::Objects::ObjectType getType() const;


        void setIndex( const std::size_t index_ );
        std::size_t getIndex() const;

        void setTreeIndex( const std::size_t index_ );
        std::size_t getTreeIndex() const;



    protected:


        std::size_t index;
        std::size_t tree_index;
        Settings::Objects::ObjectType type;


};

#endif // OBJECTTREEITEM_H
