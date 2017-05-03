#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>

#include "ObjectTreeItem.h"


class ObjectTree :public QTreeWidget
{
    Q_OBJECT

    public:

        ObjectTree( QWidget* parent = 0 );


        void addInputVolume( ObjectTreeItem* const& vol_ );
        void addObject( ObjectTreeItem* const& obj_ );
        void addRegion( ObjectTreeItem* const& reg_ );


    signals:
             void setInputVolumeVisible( bool status_ );
             void setObjectVisible( std::size_t id_, bool status_ );

    private:

        std::map< std::size_t, ObjectTreeItem* > objects;
        std::map< std::size_t, ObjectTreeItem* > regions;

        ObjectTreeItem* input_volume;
        ObjectTreeItem* output_volume;
};

#endif // OBJECTTREE_H
