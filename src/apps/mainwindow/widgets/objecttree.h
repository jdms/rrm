#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>

#include "objecttreeitem.h"
#include "./core/base/models/container.h"



class ObjectTree: public QTreeWidget
{
    Q_OBJECT


    public:

        ObjectTree( QWidget* parent = 0 );

        void addInputVolume();
        void addOutputVolume();


        void addObject( std::size_t index_, const ObjectTreeItem::Type& type_,
                        const std::string& name_,  const int& red_,
                        const int& green_,  const int& blue_ );
        void removeObject( std::size_t index_ );

        void updateObjectColor( std::size_t index_, int red_, int green_, int blue_);
        void setObjectVisibility( std::size_t index_, bool status_ );


        void addRegion( std::size_t index_, const std::string& name_,  const int& red_,
                        const int& green_,  const int& blue_ );
        void updateRegionColor( std::size_t index_, int red_, int green_, int blue_);
        void setRegionVisibility( std::size_t index_, bool status_ );


        void clear();



    protected slots:

        void filterAction( QTreeWidgetItem* item_, std::size_t column_ );


    signals:


        void setVolumeVisible( std::size_t index_, bool status_ );
        void setVolumeName( std::size_t index_, const std::string& name_ );

        void setObjectVisible( std::size_t index_, bool status_ );
        void setObjectName( std::size_t index_, const std::string& name_ );
        void setObjectColor( std::size_t index_, const QColor& color_ );

        void setCrossSectionVisible( std::size_t index_, bool status_ );
        void setCrossSectionName( std::size_t index_, const std::string& name_ );

        void setRegionVisible(  std::size_t index_, bool status_ );
        void setRegionName( std::size_t index_, const std::string& name_ );
        void setRegionColor( std::size_t index_, const QColor& color_ );



    private:

        const int COLUMNS_NUMBER = 3;
        const int COLUMN_STATUS = 0;
        const int COLUMN_NAME = 1;
        const int COLUMN_COLOR = 2;

        Container< std::size_t, ObjectTreeItem* > items;
        Container< std::size_t, ObjectTreeItem* > regions;


};

#endif // OBJECTTREE_H
