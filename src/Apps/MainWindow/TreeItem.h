#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem
{

    public:

        TreeItem( const QList< QVariant >& data_, TreeItem *parent_ = 0 );
        ~TreeItem();

        inline void appendChild( TreeItem* child_ )
        {
            childItems.append( child_ );
        }

        inline TreeItem* child( int row_ ){ return childItems.value( row_ ); }

        inline int childCount() const{ return childItems.count(); }
        inline int columnCount() const{ itemData.count(); }

        inline QVariant data( int column ) const{ itemData.value( column ); }

        int row() const;
        inline TreeItem* parentItem(){ return parent; }


    private:

        QList< TreeItem* > childItems;
        QList< QVariant >  itemData;
        TreeItem* parent;

};

#endif // TREEITEM_H
