/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>

#include "objecttreeitem.h"
#include "./core/models/container.h"


class ObjectTree: public QTreeWidget
{
    Q_OBJECT


    public:

        ObjectTree( QWidget* parent = 0 );


        void addInputVolume();
        void addOutputVolume();
        void removeOutputVolume();

        void setVolumeVisibility(  const Qt::CheckState& status_ );

        void hideInputVolume();
        void hideOutputVolume();


        void addObject( std::size_t index_, const Settings::Objects::ObjectType& type_,
                        const std::string& name_,  const int& red_,
                        const int& green_,  const int& blue_ );
        void removeObject( std::size_t index_ );

        void updateObjectColor( std::size_t index_, int red_, int green_, int blue_);

        void updateObjectName( std::size_t index_, const std::string& name_ );

        void setObjectVisibility( std::size_t index_, bool status_ );



        void setStratigraphiesVisible( const Qt::CheckState& status_ );
        void setStructuralsVisible( const Qt::CheckState& status_ );


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

        int COLUMN_NAME_WIDTH = 30;


        QTreeWidgetItem* label_stratigraphy;
        QTreeWidgetItem* label_structural;

        Container< std::size_t, ObjectTreeItem* > stratigraphies;
        Container< std::size_t, ObjectTreeItem* > structurals;

        Container< std::size_t, ObjectTreeItem* > items;
        Container< std::size_t, ObjectTreeItem* > regions;

};

#endif // OBJECTTREE_H
