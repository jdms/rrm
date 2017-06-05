#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>

#include "ObjectTreeItem.h"

class ColorPicker;

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


        void addObject( const std::size_t& id, int r, int g, int b );


        inline void setObjectHidden( const std::size_t& id_, bool status_ )
        {
            setItemHidden( objects[ id_ ], status_ );
        }


        ObjectTreeItem* addVolumeChilds( const ObjectTreeItem::TreeItemType& type, std::size_t id );


        void addRegion( const std::size_t& id );

        inline void setRegionHidden( const std::size_t id_, bool status_ )
        {
            setItemHidden( regions[ id_ ], status_ );
        }

        inline void setRegionName( const std::size_t& id_, const QString& name_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return;

            ObjectTreeItem* item_ = regions[ id_ ];
            item_->setText( 1, name_ );

        }

    signals:

        void setInputVolumeVisible( bool status_ );
        void setObjectVisible( std::size_t id_, bool status_ );
        void setObjectName( std::size_t id_, const QString& n );
        void setObjectColor( std::size_t id_, const QColor& c );


    protected slots:

        void ObjectTree::filterAction( QTreeWidgetItem* item, std::size_t column );

    private:

        std::map< std::size_t, ObjectTreeItem* > objects;
        std::map< std::size_t, ObjectTreeItem* > regions;

        ObjectTreeItem* input_volume;
        ObjectTreeItem* output_volume;
};

#endif // OBJECTTREE_H
