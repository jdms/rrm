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


        void addObject( std::size_t index_, const ObjectTreeItem::Type& type_,
                        const std::string& name_,  const int& red_,
                        const int& green_,  const int& blue_ );


//        std::string& getObjectName() const;

//        void setObjectColor( const int& red_, const int& green_,
//                             const int& blue_ );
//        void getObjectColor( int& red_, int& green_, int& blue_ ) const;

//        void setObjectVisibility( bool status_ );
//        bool isObjectVisible() const;

//        void setObjectActive( bool status_ );
//        bool isObjectActive() const;






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



    private:

        const int COLUMNS_NUMBER = 3;
        const int COLUMN_STATUS = 0;
        const int COLUMN_NAME = 1;
        const int COLUMN_COLOR = 2;

        Container< std::size_t, ObjectTreeItem* > items;


};

#endif // OBJECTTREE_H
