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
#include <QInputDialog>

#include "objecttreeitem.h"
#include "./core/models/container.h"


/**
 *  An extension of QTreeWidget to illustrate the objects presents in the RRM model scene.
 */


class ObjectTree: public QTreeWidget
{
    Q_OBJECT


    public:


        /**
        * Constructor.
        */
        ObjectTree( QWidget* parent = nullptr );


        /**
        * Method to add a volume in the object tree.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addInputVolume();


        /**
        * Method to add a volume derived from the volume regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addOutputVolume();


        /**
        * Method to remove the volume derived from the volume regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeOutputVolume();


        /**
        * Method to change the visibility of any of the volumes
        * @param index_ index of the volume. If index_ is 0, the volume is the input, and if
        * the value is 1 the volume is the volume derived from the volume regions
        * @param status_ if status_ is 'CHECKED' the volume is visible and 'UNCHECKED' otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeVisibility(  std::size_t index_, const Qt::CheckState& status_ );


        /**
        * Method to show the total volume of input volume
        * @param volume_ the total volume of the input volume, i.e., the sum of the volume of all regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setTotalVolume( double volume_ );


        /**
        * Method to add a new stratigraphy or structural in the object tree
        * @param index_ index of the stratigraphy/structural
        * @param type_ object type, if it is a 'STRATIGRAPHY' or a 'STRUCTURAL'
        * @param name_ the name of the stratigraphy/structural
        * @param red_ the red component of the stratigraphy/structural color
        * @param green_ the green component of the stratigraphy/structural color
        * @param blue_ the blue component of the stratigraphy/structural color
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addObject( std::size_t index_, const Settings::Objects::ObjectType& type_,
                        const std::string& name_,  const int& red_,
                        const int& green_,  const int& blue_ );


        /**
        * Method to remove a stratigraphy or structural from the object tree.
        * @param index_ index of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeObject( std::size_t index_ );


        /**
        * Method to update a stratigraphy or structural color in the object tree.
        * @param index_ index of the stratigraphy/structural
        * @param red_ the red component of the stratigraphy/structural color
        * @param green_ the green component of the stratigraphy/structural color
        * @param blue_ the blue component of the stratigraphy/structural color
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateObjectColor( std::size_t index_, int red_, int green_, int blue_);


        /**
        * Method to update a stratigraphy or structural name in the object tree.
        * @param index_ index of the stratigraphy/structural
        * @param name_ the new name of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateObjectName( std::size_t index_, const std::string& name_ );


        /**
        * Method to change the visibility of a stratigraphy or structural
        * @param index_ index of the stratigraphy/structural
        * @param status_ if status_ is true the stratigraphy/structural is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectVisibility( std::size_t index_, bool status_ );


        /**
        * Method to change the visibility of the stratigraphies
        * @param status_ if status_ is 'CHECKED' the stratigraphies are visible and 'UNCHECKED' otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setStratigraphiesVisible( const Qt::CheckState& status_ );


        /**
        * Method to change the visibility of the structurals
        * @param status_ if status_ is 'CHECKED' the structurals are visible and 'UNCHECKED' otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setStructuralsVisible( const Qt::CheckState& status_ );


        /**
        * Method to remove all stratigraphies from the object tree.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeStratigraphies();


        /**
        * Method to remove all structurals from the object tree.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeStructurals();


        /**
        * Method to add a new region in the object tree
        * @param index_ index of the region
        * @param name_ the name of the stratigraphy/structural
        * @param red_ the red component of the stratigraphy/structural color
        * @param green_ the green component of the stratigraphy/structural color
        * @param blue_ the blue component of the stratigraphy/structural color
        * @param volume_ the volume of the region
        * @param perc_ the percentage of the region volume compared with the total
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegion(std::size_t index_, const std::string& name_,  const int& red_, const int& green_,  const int& blue_, double volume_ , double perc_ );


        /**
        * Method to update a region in the object tree
        * @param index_ index of the region
        * @param red_ the red component of the stratigraphy/structural color
        * @param green_ the green component of the stratigraphy/structural color
        * @param blue_ the blue component of the stratigraphy/structural color
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateRegionColor( std::size_t index_, int red_, int green_, int blue_);


        /**
        * Method to change the visibility of a region
        * @param index_ index of the region
        * @param status_ if status_ is true the region is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setRegionVisibility( std::size_t index_, bool status_ );


        /**
        * Method to change the visibility of the domains
        * @param status_ if status_ is 'CHECKED' the domains are visible and 'UNCHECKED' otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setDomainsVisible( const Qt::CheckState& state_ );


        /**
        * Method to change the visibility of a domain and its regions
        * @param index_ index of the domain
        * @param status_ if status_ is true' the domain is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setDomainsVisibility( std::size_t index_, bool status_ );


        /**
        * Method to update a domain and its regions
        * @param index_ index of the domain
        * @param volume_ the volume of the domain, i.e, the sum of the regions volumes that consists the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateVolumeDomain( std::size_t index_, double volume_ );



    public slots:


        /**
        * Method (slot) to remove all elements from the object tree
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clear();


        /**
        * Method (slot) to create the menu
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createMenu();


        /**
        * Method (slot) to clear the submenu related to domains
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clearSubMenu();


        /**
        * Method (slot) to remove all regions from the object tree
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeRegions();


        /**
        * Method (slot) to add a domain in the object tree
        * @param index_ index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        bool createDomain( std::size_t index_ );


        /**
        * Method (slot) to retrieve a vector of regions selected and another vector with their domains
        * @param regions_ vector of index of the regions selected
        * @param parents_ vector with the indexes of the domains, which regions was selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        bool getSelectedRegionsList( std::vector< std::size_t >& regions_, std::vector< std::size_t >& parents_ );


        /**
        * Method (slot) to retrieve a vector of regions selected
        * @param regions_ vector of index of regions selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        bool getSelectedRegionsList( std::vector< std::size_t >& regions_ );


        /**
        * Method (slot) to add regions into a existent domain
        * @param index_ index of the domain
        * @param regions_ vector of index of regions selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegionsInDomain( std::size_t index_, const std::vector< std::size_t >& regions_ );


        /**
        * Method (slot) to add regions into a existent domain
        * @param index_ index of the domain
        * @param regions_ set of index of regions selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegionsInDomain( std::size_t index_, const std::set< std::size_t >& regions_ );


        /**
        * Method (slot) to add a selected region into a existent domain
        * @param index_ index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addToDomain( std::size_t index_ );


        /**
        * Method (slot) to remove all domains from the object tree
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeDomains();


        /**
        * Method (slot) to remove and delete a set of regions from their domains
        * @param regions_ vector of index of regions selected
        * @param parents_ vector with indexes of the domains, which regions was selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeRegionsOfTheirDomains( const std::vector< std::size_t >& regions_,
                                           const std::vector< std::size_t >& parents_ );


        /**
        * Method (slot) to remove without deleting a set of regions from their domains
        * @param regions_ vector of index of regions selected
        * @param parents_ vector with indexes of the domains, which regions was selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeRegionsOfTheirDomainsNoDelete( const std::vector< std::size_t >& regions_,
                                           const std::vector< std::size_t >& parents_ );


        /**
        * Method (slot) to get the selected regions and call the signal 'removeRegionsFromTheirDomains' to remove them from their domains
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeFromDomain();


        /**
        * Method (slot) to delete a domain that was selected or which index is index_
        * @param index_ option size_t to indicate the index of the domain. If the parameter is not passed,
        * it is supposed that the domain was selected
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void deleteDomain( std::size_t index_ = std::numeric_limits<std::size_t>::max() );


        /**
        * Method (slot) to delete selected domains
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void deleteDomains();


        /**
        * Method (slot) to sort the stratigraphies and structurals following the 'natural' order
        * @param indexes_ indexes of the the stratigraphies and structurals previously ordered
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void sortStratigraphies( std::vector< std::size_t > indexes_ );


        /**
        * Method (slot) to sort the domains based in the regions indexes
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void sortDomains();


        /**
        * Method (slot) to send the input log of a stratigraphy/structural to controller
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void sendSurfaceLog();


    protected slots:


        /**
        * Method (slot) called when a change happens in any object from the object tree
        * @param item_ the object changed
        * @param column_ the column of the object that was changed
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void filterAction( QTreeWidgetItem* item_, std::size_t column_ );


        /**
        * Method (slot) called when the user clicks over an object from the object tree
        * @param item_ the object clicked
        * @param column_ the column of the object that was clicked
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clickAction( QTreeWidgetItem* item_, std::size_t column_ );


        /**
        * Method (slot) to show the menu
        * @param pos_  position where the origin of the menu should be placed
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void showMenu( const QPoint& pos_ );


        /**
        * Method (slot) to delete the widget that is placed in the column given of the object
        * @param obj_  the object which widget should be removed
        * @param column_ the column of the object which the widget is placed
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void deleteWidgetFromObject( ObjectTreeItem* obj_, int column_ );


        /**
        * Method (slot) to remove the input volume from the object tree
        * Once the volume should be removed, all the stratigraphies and structurals as its dependent,
        * should be removed as well.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeInputVolume();



    signals:


        /**
        * Signal to notify that the volume visibility has changed
        * @param index_ index of the volume
        * @param status_ if the value of status is true, the volume is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeVisible( std::size_t index_, bool status_ );


        /**
        * Signal to notify that the volume name has changed
        * @param index_ index of the volume
        * @param name_ new name of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeName( std::size_t index_, const std::string& name_ );


        /**
        * Signal to notify that the visibility of a stratigraphy or structural has changed
        * @param index_ index of the stratigraphy/structural
        * @param status_ if the value of status is true, the stratigraphy/structural is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectVisible( std::size_t index_, bool status_ );


        /**
        * Signal to notify that the name of a stratigraphy or structural has changed
        * @param index_ index of the stratigraphy/structural
        * @param name_ new name of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectName( std::size_t index_, const std::string& name_ );


        /**
        * Signal to notify that the color of a stratigraphy or structural has changed
        * @param index_ index of the stratigraphy/structural
        * @param color new color of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectColor( std::size_t index_, const QColor& color_ );


        /**
        * Signal to notify that the visibility of a cross-section has changed
        * @param index_ index of the cross-section
        * @param status_ if the value of status is true, the cross-section is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setCrossSectionVisible( std::size_t index_, bool status_ );


        /**
        * Signal to notify that the name of a cross-section has changed
        * @param index_ index of the cross-section
        * @param name_ new name of the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setCrossSectionName( std::size_t index_, const std::string& name_ );


        /**
        * Signal to notify that the visibility of a region has changed
        * @param index_ index of the region
        * @param status_ if the value of status is true, the region is visible and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setRegionVisible(  std::size_t index_, bool status_ );


        /**
        * Signal to notify that the name of a region has changed
        * @param index_ index of the region
        * @param name_ new name of the region
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setRegionName( std::size_t index_, const std::string& name_ );


        /**
        * Signal to notify that the color of a region has changed
        * @param index_ index of the region
        * @param color new color of the region
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setRegionColor( std::size_t index_, const QColor& color_ );


        /**
        * Signal to notify that an object was selected
        * @param type_ the type of the object, i.e., it can be 'NONE, 'VOLUME', 'CROSS_SECTION',
        * 'STRATIGRAPHY', 'STRUCTURAL', 'REGION', or 'DOMAINS'
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void objectSelected( const Settings::Objects::ObjectType& type_ );


        /**
        * Signal emmited when the user ask to create a domain from selected regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createDomainOfRegions();


        /**
        * Signal emmited when the user ask to create a domain from selected regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegionToDomain( std::size_t reg_id_, std::size_t domain_id_ );


        /**
        * Signal to notify that the region which index is reg_id_ should be removed from the domain domain_id_
        * @param reg_id_ index of the region
        * @param domain_id_ index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeRegionFromDomain( std::size_t reg_id_, std::size_t domain_id_ );


        /**
        * Signal to notify that a domain should be removed
        * @param index_ index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeDomain( std::size_t index_ );


        /**
        * Signal to notify that the set of regions regions_ should be added to the domain which index is index_
        * @param index_ index of the domain
        * @param regions_ set of regions to be added to the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegionsToDomain( std::size_t index_, const std::vector< std::size_t >& regions_ );


        /**
        * Signal to notify that the set of regions regions_ should be removed and deleted from their domains
        * @param regions_ set of indexes of regions to be deleted
        * @param domains_ set of indexes of domains which the regions belong
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeRegionsFromTheirDomains( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_ );


        /**
        * Signal to notify that the set of regions regions_ should be removed, without deleting, from their domains
        * @param regions_ set of indexes of regions to be deleted
        * @param domains_ set of indexes of domains which the regions belong
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeRegionsFromTheirDomainsNoDelete( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_ );


        /**
        * Signal to notify that the name of a domain was changed
        * @param index the index of the domain
        * @param name_ the new name of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setDomainName( std::size_t index_, const std::string& name_ );


        /**
        * Signal to notify that the color of a domain was changed
        * @param index the index of the domain
        * @param color_ the new color of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setDomainColor( std::size_t index_, const QColor& color_ );


        /**
        * Signal to notify the controller that the log of a stratigraphy or structural should be save
        * @param index the index of the stratigraphy/structural
        * @param log_ the data log of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void saveSurfaceLog( int index_, const QString& log_ );


        /**
        * Signal to retrieve the log of a stratigraphy or structural
        * @param index the index of the stratigraphy/structural
        * @param log_ the data log of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void getSurfaceLog( int index_, QString& log_ );


    private:

        const int COLUMNS_NUMBER = 5;                                               /**< Number of columns of each row */

        const int COLUMN_STATUS = 0;                                                /**< Index of the column which is related to the object visibility */

        const int COLUMN_NAME = 1;                                                  /**< Index of the column which is related to the object name */

        const int COLUMN_COLOR = 2;                                                 /**< Index of the column which is related to the object color */

        const int COLUMN_DETAILS = 3;                                               /**< Index of the column which is related to the object details, e.g., the volume of the regions, domains and volume */

        int COLUMN_NAME_WIDTH = 30;                                                 /**< The width of the column related to the object name */

        QTreeWidgetItem* label_stratigraphy;                                        /**< Parent item to stratigraphies objects */

        QTreeWidgetItem* label_structural;                                          /**< Parent item to structural objects */

        QTreeWidgetItem* label_domains;                                             /**< Parent item to regions objects */

        Container< std::size_t, ObjectTreeItem* > stratigraphies;                   /**< Container of stratigraphies. The first value is the index of the stratigraphy */

        Container< std::size_t, ObjectTreeItem* > structurals;                      /**< Container of structurals. The first value is the index of the structural */

        Container< std::size_t, ObjectTreeItem* > items;                            /**< Container of items, e.g., all the objects. The first value is the index of the object */

        Container< std::size_t, ObjectTreeItem* > regions;                          /**< Container of regions. The first value is the index of the regions */

        Container< std::size_t, ObjectTreeItem* > domains;                          /**< Container of domains. The first value is the index of the domains */

        std::map< std::size_t, QAction* > domain_actions_;                          /**< When a domain is created, an action is associated to it. This map allows to map each to other */

        std::map< std::size_t, double > volume_regions;                             /**< Mapping between the regions and theirs respective volumes */

        std::map< std::size_t, double > volume_domains;                             /**< Mapping between the domains and theirs respective volumes */

        QMenu* mn_menu = nullptr;                                                   /**< The context menu. It appears when the user uses a right-click over the object tree */

        QMenu* mn_submenu = nullptr;                                                /**< A submenu which contains an action to each existent domain */

        QAction* ac_create_domain = nullptr;                                        /**< An action to create a domain from selected regions */

        QAction* ac_remove_domain = nullptr;                                        /**< An action to remove a domain */

        QAction* ac_removefrom_domain = nullptr;                                    /**< An action to remove regions from their domains */

        QAction* ac_addto_domain = nullptr;                                         /**< An action to add regions to adomains */

        QInputDialog* wg_log_ = nullptr;                                            /**< A dialog with a text input to user enters a log information to a stratigraphy or structural  */

        int surface_selected_ = -1;                                                 /**< An integer to save the index of the surface which was selected */

        bool is_perc = false;                                                       /**< A boolean to indicates if the volumes should be shown in its total value or its percentage */
};

#endif // OBJECTTREE_H
