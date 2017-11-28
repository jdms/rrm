#ifndef OBJECTTREEITEM_H
#define OBJECTTREEITEM_H


#include <QTreeWidgetItem>

#include "./core/base/constants/constants.hpp"


class ObjectTreeItem: public QTreeWidgetItem
{
    public:

//        enum class Type { VOLUME, CROSS_SECTION, STRATIGRAPHY, REGION, WELL };


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
