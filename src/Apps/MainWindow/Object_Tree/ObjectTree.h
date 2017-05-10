#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>

#include "ObjectTreeItem.h"


class ObjectTree :public QTreeWidget
{
    Q_OBJECT

    public:

        ObjectTree( QWidget* parent = 0 );


        void addInputVolume();

        inline void setInputVolumeHidden( bool status_ )
        {
            setItemHidden( input_volume, status_ );
        }


        void addObject( const std::size_t& id_ );

        inline void setObjectHidden( const std::size_t& id_, bool status_ )
        {
            setItemHidden( objects[ id_ ], status_ );
        }


        void addRegion( const std::size_t& id_ );

        inline void setRegionHidden( const std::size_t id_, bool status_ )
        {
            setItemHidden( regions[ id_ ], status_ );
        }


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
