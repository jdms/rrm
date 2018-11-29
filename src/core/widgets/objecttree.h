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

#include <set>
#include <numeric>

#include <QTreeWidget>
#include <QMenu>
#include<QDragEnterEvent>
#include<QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>

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

        void setVolumeVisibility(  std::size_t index_, const Qt::CheckState& status_ );

        void hideInputVolume();
        void hideOutputVolume();

        void setTotalVolume( double volume_ );


        void addObject( std::size_t index_, const Settings::Objects::ObjectType& type_,
                        const std::string& name_,  const int& red_,
                        const int& green_,  const int& blue_ );
        void removeObject( std::size_t index_ );

        void updateObjectColor( std::size_t index_, int red_, int green_, int blue_);

        void updateObjectName( std::size_t index_, const std::string& name_ );

        void setObjectVisibility( std::size_t index_, bool status_ );



        void setStratigraphiesVisible( const Qt::CheckState& status_ );
        void setStructuralsVisible( const Qt::CheckState& status_ );
        void setDomainsVisible( const Qt::CheckState& state_ );


        void removeStratigraphies();
        void removeStructurals();


        void addRegion(std::size_t index_, const std::string& name_,  const int& red_, const int& green_,  const int& blue_, double volume_ , double perc_ );

        void updateRegionColor( std::size_t index_, int red_, int green_, int blue_);
        void setRegionVisibility( std::size_t index_, bool status_ );

        void setDomainsVisibility( std::size_t index_, bool status_ );



//    private:

//        void dragEnterEvent( QDragEnterEvent* event );
//        void dragMoveEvent( QDragMoveEvent* event );
//        void dropEvent( QDropEvent *event );
//        void mouseReleaseEvent( QMouseEvent *event );
//        void mouseMoveEvent( QMouseEvent *event );



    public slots:

        void clear();
        void createMenu();
        void clearSubMenu();

        void createDomain( std::size_t index_ = 0 );
        void deleteDomain( std::size_t index_ = 0 );
        void removeFromDomain();
        void removeDomains();
        void addToDomain( std::size_t index_ );

        void removeRegions();


        bool createDomain1( std::size_t index_ );
        bool getSelectedRegionsList( std::vector< std::size_t >& regions_, std::vector< std::size_t >& parents_ = std::vector< std::size_t >() );
        void addRegionsInDomain( std::size_t index_, const std::vector< std::size_t >& regions_ );
        void addRegionsInDomain( std::size_t index_, const std::set< std::size_t >& regions_ );
        void addToDomain1( std::size_t index_ );
        void removeRegionsOfTheirDomains1( const std::vector< std::size_t >& regions_,
                                           const std::vector< std::size_t >& parents_ );
        void removeRegionsOfTheirDomainsNoDelete( const std::vector< std::size_t >& regions_,
                                           const std::vector< std::size_t >& parents_ );


        void removeFromDomain1();
        void deleteDomain1( std::size_t index_ = std::numeric_limits<std::size_t>::max() );
        void deleteDomains1();

        void sortStratigraphies( std::vector< std::size_t > indexes_ );

        void sortDomains();


    protected slots:

        void filterAction( QTreeWidgetItem* item_, std::size_t column_ );
        void clickAction( QTreeWidgetItem* item_, std::size_t column_ );

        void showMenu( const QPoint& pos_ );

        void deleteWidgetFromObject( ObjectTreeItem* obj_, int column_ );
        void removeInputVolume();






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

        void objectSelected( const Settings::Objects::ObjectType& type_ );


        void createDomainOfRegions();
        void addRegionToDomain( std::size_t reg_id_, std::size_t domain_id_ );
        void removeRegionFromDomain( std::size_t reg_id_, std::size_t domain_id_ );
        void removeDomain( std::size_t index_ );


        void addRegionsToDomain( std::size_t index_, const std::vector< std::size_t >& regions_ );
        void removeRegionsFromTheirDomains( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_ );
        void removeRegionsFromTheirDomainsNoDelete( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_ );


        void setDomainName( std::size_t index_, const std::string& name_ );
        void setDomainColor( std::size_t index_, const QColor& color_ );


    private:

        const int COLUMNS_NUMBER = 5;
        const int COLUMN_STATUS = 0;
        const int COLUMN_NAME = 1;
        const int COLUMN_COLOR = 2;
        const int COLUMN_DETAILS = 3;

        int COLUMN_NAME_WIDTH = 30;


        QTreeWidgetItem* label_stratigraphy;
        QTreeWidgetItem* label_structural;
        QTreeWidgetItem* label_domains;

        Container< std::size_t, ObjectTreeItem* > stratigraphies;
        Container< std::size_t, ObjectTreeItem* > structurals;

        Container< std::size_t, ObjectTreeItem* > items;
        Container< std::size_t, ObjectTreeItem* > regions;
        Container< std::size_t, ObjectTreeItem* > domains;

        std::map< std::size_t, QAction* > domain_actions_;

        std::map< std::size_t, double > volume_regions;


        QMenu* mn_menu = nullptr;
        QMenu* mn_submenu = nullptr;
        QAction* ac_create_domain = nullptr;
        QAction* ac_remove_domain = nullptr;
        QAction* ac_removefrom_domain = nullptr;
        QAction* ac_addto_domain = nullptr;

        bool is_perc = false;
};

#endif // OBJECTTREE_H
